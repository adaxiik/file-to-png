# file-to-png
Small C program to convert any file to .png

## Dependencies
* libpng 

## Usage
```bash
$ filetopng <mode> <input> <output>
```
Mode | Description
---- | -----------
-t | Convert to png
-f | Convert from png

Usage example:
```$ filetopng -t input.txt output.png```

## Example
- Program itself in png:

    ![example1](examples/filetopngaspng.png)

- penguin.jpg in png:

    ![example2](examples/penguinjpg.png)