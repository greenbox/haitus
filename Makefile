SUBDIRS = src

all:
	for x in $(SUBDIRS); do make -C $$x; done
clean:
	for x in $(SUBDIRS); do make -C $$x clean; done
