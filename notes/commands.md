Question of the day: should it be possible to `cd` into a feed?

The following is idealized and not neccessarily documentation of what acutalyl happens

----

# Command list:

## exit (quit program)

## echo (echo parameters)

## cd (change directory)

    cd %abstract_path%

The feed storage has folders and such similar to a file system, so `cd` exists similar to a file system.

At startup the current location is set to the root directory.

## dir (print contents)

    dir [-n] [%abstract_path%]

* -n is a flag that, when set, indicates that dir should only show entries with new data. Folders are always shown, even with this flag.

prints the contents of the directory or feed represented by the abstract_path.
If the abstract_path is missing, the contents of the current directory are
displayed.

?? also if the path points to a feed item?

## md (make directory)

    md %abstract_path%

Creates a directory at the specified path. fails if something exists at that location.

## subscribe (suscribe to a feed)

    subscribe %abstract_path% %feed_url%

Creates a feed subscription at the specified path for the specified url.

## del (delete a feed or directory)

    del %abstract_path%

## markAsRead (mark an item as read)

    markAsRead %abstract_path%

Mark an item or feed as read

?? better name

## move (move)

    markAsRead %abstract_path% %abstract_path%

The first parameter is the source; the second parameter is the destination

## displayItem (display a feed item)

    displayItem %abstract_path%

Does not mark that item as read

?? better name
?? also display detailed data about a feed?

## openAttachment (open a subprocess to display a feed item's attachment)

Does not mark that item as read

## openItem (open a subproccess to display a feed item)

Does not mark that item as read



----

`[]` indicates an optional paramter.

`%abstract_path%` is a string representing a path. Either `/` or `\` may be used
to separate elements in a path. The path is always relative to the current
directory.

`%feed_url%` must be an absolute URL that begins with the "http:" or "https:" protocol. No other protocols are supported.
