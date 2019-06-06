all: escapeless

TESTS = ../tests
TESTER = ./tester.py

escapeless:
	pep8 escapeless.py
	python setup.py sdist

tester: $(TESTER)
	pep8 tester.py

clean:
	python setup.py clean
	-rm -rf dist
	-rm -rf escapeless.egg-info
	-rm -rf escapeless.pyc

include $(TESTS)/Makefile.inc
