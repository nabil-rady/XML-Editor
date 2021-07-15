# XML-Editor

QT desktop application that lets you edit XML file and do various operations on it.

## How does it work? 

The application parses the XML file and then builds a tree of nodes, each node represesnts and XML tag (or XML attribute), after that the program can use the tree in order to execute other algorithms.

## Convert to JSON

You can enter an XML file and convert it to JSON if the file is consistent.

## Error highlighting and fixing

If your XML file is missing and opening tag or has a syntactic error the program will the highlight the mistake for you and offer you the option to solve this issue.

## Compression and Decompression

The Application uses LZ77 algorithm to compress any file to ZXML extension and allows you to decompress the ZXML file as well.

## XML manipulation

You can also manipulate the XML file, either by beautifying the XML by adding clear indentations or minifiying the file by removeing unnecessary spaces and new lines.