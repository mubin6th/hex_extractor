## hex_extractor

### About
`hex_extractor` is a tool to extract space separated hex codes
and places them in an image palette. It's useful as it gives you
an idea of what colors are inside a file instead of rough guesses.


The code for the program is primarily in the `main.c`. The `libbmp` files
are here to read and write files. I have choosen to write as little code
as possible for the job.


You can run the program from any terminal. Make sure you have built the
project and then you can run it like so, `./hex_extractor <options>`.

### Building
There is no linking of shared libraries. Meaning there should not be
errors when building. Make sure you have the `gcc` compiler. If you have
any other compiler, then change the `Makefile`'s `COMPILER` option to
your preference. In your terminal, at the root of the project,
run: `make`

### Options
If will soon (Insha-allah) add the feature to view all the options.
It will show the options when you run `./hex_extractor --help` or
`./hex_extractor -h`
