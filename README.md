# dir-mapper
CLI tool to manage file conversion between two directories, with parallel conversion and timestamp check

This was created to automate my management of iTunes imported music library, which contains alac(apple lossless) encoded m4a files.

### Dependencies
To compile: compiler version supporting c++20, CMake version >= 4.0

To run: have command line `ffmpeg, ffprobe, md5sum`. If this is used to only copy files over, then none of these are required. 

On windows: untested. Most of the code should be platform independent, given that everything is using stdlib. 
I'm unsure if getting `md5sum` onto windows is possible, but `ffmpeg` and `ffprobe` should be. 

### Usage

Recommended running of the command looks like this:
```shell
--input-path="/path/to/input"
--output-path="/path/to/output"
--enable-alac-conversion
--handle-alac-year
--cleanup
-t16
```

This command looks at all files, if any file is m4a with alac encoding, convert them to flac in the target directory. For any other file, it gets copied over when necessary. 

- `input-path` Specifies the path to input, which should be a directory containing the iTunes managed music.
- `output-path` Specifies where to put the files, if a directory does not exist, one will be created. 
- `enable-alac-conversion` When specified, treats `.m4a` files as possible conversion candidate, runs `ffprobe` against them, and when the codec is alac (instead of say aac), converts the file to flac format using `ffmpeg`.
- `handle-alac-year` Depends on `enable-alac-conversion`. When specified, for all alac fils, run additional `ffprobe` to find the year metadata. Due to iTunes using `date` as field for year, converted flac files will not show the music's year correctly in many devices. Enabling this option will inject the same value into `year` field in the flac file, which will be recognized by most players. 
- `enable-hash-check` When not specified, a file copy happens if either the target file does not exist, or the target file have an older timestamp than source. When specified, copying over the files will only be skipped if the hash also match. On initial runs, specifying this is likely a waste of time. Only enable this is you think some files might be corrupted. 
- `cleanup` When specified, deletes files and directories in the target directory that isn't accounted for by the input. If a flac file had a corresponding original m4a alac file, or when other files are copied from a corresponding file in the input, it is accounted for. Other files are not. 
- `t16` Specifies 16 concurrent runs of work steps, say running any command (`ffprobe, ffmpeg, md5sum`) and copying file. You can specify the value to be proportional to your cpu count, say `t8`, `t32`. 
- `dryrun` When specified, take no action. 
- `debug` When specified, prints detailed debug info. 

If none of `enable-alac-conversion`, `handle-alac-year`, `enable-hash-check` are specified, a run is similar in effect to a local `rsync` run as it compares recursively the timestamps of files, and if file is missing or older than source, copy it over.

Under this condition, when `enable-hash-check` and `cleanup` gets specified, it essentially performs a content comparing file sync operation.

In concept thi

### Performance 
With my files sitting at around 100 GB, one full conversion (however, around 20% of file count, which made up 10% of the file size, simply got copied over) 
took 6 minutes and 12 seconds. An audit run (without hash comparison, but with alac probings) took less than 1 minute. 

Since this was done on a desktop CPU + relatively fast SSDs, your mileage may vary since this is rather CPU and IO intensive. 