FILESOURCE= InstallStringTable.txt
FILETARGET= InstallStringTable.utf8

all:
	sed 's/\x22\x00\x22\x00/\x22\x00/g' $(FILESOURCE) | iconv -f UTF16 -t UTF8 > $(FILETARGET)
	dos2unix $(FILETARGET)
	sed '1s/^\x09/\/\//; 1s/\x09/ /g; 1!s/^/xx(/g; 1!s/$$/")\,/g; 1!s/\x09/","/g; s/",/,/' -i $(FILETARGET)
	# first line, add // and replace 0x09 with space.
	# other lines:
	# add xx( at the beginning, add "), at the end.
	# replace 0x09 with ","
	# replace first ", with ,

clean:
	rm $(FILETARGET)
