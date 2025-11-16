"""Utilities to generate and build with old MSVC 1.5. We would prefer to use CMake, but it is not compatible."""

import re
from pathlib import Path
from typing import Iterable, Iterator


# Get any quoted includes that are on their own line.
r_include = re.compile(rb'(?:\A|\n)\s*#include "(\S+)"')


def get_deps_for_file(src_path: Path) -> Iterator[Path]:
    """Return the headers from any '#include "header"' lines in the code file at the given path."""
    with open(src_path, "rb") as f:
        text = f.read()

    for b_include_path in r_include.findall(text):
        # TODO: OK to assume ASCII for 8.3 paths?
        include_path = b_include_path.decode("ascii")

        # The include might have backdir references. Produce a new absolute path.
        dep_path = (src_path.parent / include_path).resolve()

        # TODO: Maybe just warn here.
        assert dep_path.exists()
        yield dep_path


def string_continuation(lines: Iterable[str]):
    """Condense the series of lines into a single line with string continuations at each newline."""
    return " \\\n\t".join(map(str, lines))


def get_file_alias(src_path: Path, src_dir: Path) -> str:
    rel_path = src_path.relative_to(src_dir)
    parts = rel_path.with_suffix("").parts
    return "_".join([p.upper() for p in parts])


class Makefile:
    project_name: str
    obj_files: dict[Path, Path]
    files: tuple[Path, ...]
    variables: dict[str, str]
    aliases: dict[Path, str]
    deps: dict[Path, tuple[Path]] # As distinct from variables?

    def __init__(self, project_name: str) -> None:
        self.project_name = project_name
        self.files = ()
        self.variables = {}
        self.aliases = {}
        self.deps = {}

    def create_header(self) -> str:
        return "# disinvite generated build script - run build.py to regenerate!"

    def create_variables(self) -> Iterator[str]:
        if "PROJ" not in self.variables:
            yield f"PROJ = {self.project_name}"

        for key, value in self.variables.items():
            yield f"{key} = {value}"

    def create_deps(self) -> Iterator[str]:
        for file, deps in self.deps.items():
            dep_variable = self.aliases[file] + "_DEP"
            yield f"{dep_variable} = {string_continuation(deps)}"
            yield "" # Blank between each.

    def create_obj_targets(self, root: Path, src_dir: Path) -> Iterator[str]:
        yield "all:   $(PROJ).EXE"
        yield "" # Blank between each.

        for obj_file, src_file in self.obj_files.items():
            dep_variable = self.aliases[src_file] + "_DEP"
            # src_relative = src_file.relative_to(root, walk_up=True)
            src_relative = Path("..") / "src" / src_file.relative_to(src_dir)
            obj_relative = obj_file.relative_to(root)
            yield f'{obj_relative}:\t{src_relative} $({dep_variable})\n\t$(CPP) /Fo"$@" $(CFLAGS) $(CPPCREATEPCHFLAG) /c {src_relative}'
            yield "" # Blank between each.

    def create_main_targets(self, root: Path) -> Iterator[str]:
        objs_relative = [obj_file.relative_to(root) for obj_file in self.obj_files.keys()]
        objs = string_continuation(objs_relative)
        #objs_plus = "\n".join([obj + " +" for obj in objs_relative])

        yield from [
            f"$(PROJ).EXE::\t{objs} $(OBJS_EXT) $(DEFFILE)",
            "\techo >NUL @<<$(PROJ).CRF",
            *[str(obj) + " +" for obj in objs_relative],
            "$(OBJS_EXT)",
            "$(PROJ).EXE",
            "$(MAPFILE)",
            "c:\\tools\\msvc15\\lib\\+",
            "c:\\tools\\msvc15\\mfc\\lib\\+",
            "C:\\DOS\\vfwdk\\lib\\+",
            "$(LIBS)",
            "$(DEFFILE);",
            "<<",
            "\tlink $(LFLAGS) @$(PROJ).CRF",
            # "\t$(RC) $(RESFLAGS) JMAN.RES $@",
        ]


def main():
    # Ensure that we always run the build from the build subdirectory.
    project_dir = Path(__file__).parent
    build_dir = project_dir / "build"
    build_dir.mkdir(exist_ok=True)

    assert build_dir.name == "build"
    assert build_dir.is_dir()

    src_dir = project_dir / "src"
    assert src_dir.is_dir()

    # OBJ dir to keep artifact files out of the way.
    # Must create OBJ or MSVC will fail to build.
    obj_dir = build_dir / "obj"
    obj_dir.mkdir(exist_ok=True)

    # TODO: customization.
    makefile = Makefile("JMAN")
    makefile.variables.update(
        {
            "CPP": "cl",
            "DEFFILE": "..\\SRC\\JMAN.DEF",
            "CFLAGS": '/nologo /Gs /G3 /W3 /Zi /AL /Gx- /O2 /Ob1 /GA /Fd"$(PROJ).PDB" /Fm"$(PROJ).MAP"',
            "LFLAGS": "/NOLOGO /NOD /PACKC:61440 /STACK:10240 /ALIGN:16 /ONERROR:NOEXE /CO /MAP /LINE",
            "LIBS": "lafxcw llibcew vfw oldnames libw commdlg.lib mmsystem.lib shell.lib",
            "MAPFILE": "$(PROJ).MAP",
            "OBJS_EXT": "",
            "RC": "rc",
        }
    )

    # Get all source files from SRC recursively.
    makefile.files = tuple(src_dir.glob("**/*.cpp"))

    # Set up the target OBJ files for each source file.
    # Use the same directory structure as SRC.
    makefile.obj_files = {
        obj_dir / file.relative_to(src_dir).with_suffix(".obj") : file
        for file in makefile.files
    }

    # Create each subdirectory under OBJ. Required for MSVC.
    for subdir in set(file.parent for file in makefile.obj_files.keys()):
        subdir.mkdir(parents=True, exist_ok=True)

    # Create aliases for each file (replace \\ path with _).
    # This is used to create the DEP variables to list each file's dependencies.
    makefile.aliases = { file: get_file_alias(file, src_dir) for file in makefile.files }

    # Scan each source file and declare the header file dependencies.
    for file in makefile.files:
        makefile.deps[file] = tuple(get_deps_for_file(file))

    # Generate the makefile.
    outfile_lines = []

    outfile_lines.append(makefile.create_header())
    outfile_lines.append("")

    outfile_lines.extend(makefile.create_variables())
    outfile_lines.append("")

    outfile_lines.extend(makefile.create_deps())
    outfile_lines.append("")

    outfile_lines.extend(makefile.create_obj_targets(build_dir, src_dir))
    outfile_lines.append("")

    outfile_lines.extend(makefile.create_main_targets(build_dir))
    outfile_lines.append("")

    with open(build_dir / "makefile", "w+", encoding="utf-8") as f:
        f.write("\n".join(outfile_lines))


if __name__ == "__main__":
    main()
