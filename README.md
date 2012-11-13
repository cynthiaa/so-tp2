# CVS-like program for version control in Minix

## Frontend

The frontend consists of the `cvs` command, which gets passed different parameters to execute actions on the repository.

## Backend

All functions in the backend should be prefixed with `cvs_`. These will be the functions that will perform the actual manipulation of the repository.

### Data structures

The changes between revisions are added to a `changes` file in each version directory. It has the following format:

    D path/to/deleted/file
    M path/to/modified/file [diff-file-name-hash]
    A path/to/new/file      [new-file-contents-hash]
    R old/file/path -> new/file/path

