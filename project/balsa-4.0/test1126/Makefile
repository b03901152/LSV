### Balsa Makefile
### Created: Tue Nov 28 12:34:38 CST 2017
### By: yoyo@yoyoChen (Linux)
### With balsa-make-makefile version: 4.0
### Command: balsa-make-makefile -b -p /home/yoyo/Desktop/LSV/project/balsa-4.0/test1126

### Must use a Bourne shell
SHELL = /bin/sh

### Programs
BALSAC = balsa-c
BALSAMAKETEST = balsa-make-test -b
BALSAMAKEIMPLTEST = balsa-make-impl-test -b
BALSAMAKEBUILTIN = balsa-make-builtin-lib
BALSASIMIMPL = balsa-sim-impl -b
BALSANETLIST = balsa-netlist
BREEZE2PS = breeze2ps
BREEZESIM = breeze-sim
BREEZESIMCTRL = breeze-sim-ctrl
BALSAWAVE = balsa-wave
MAKE = make
RM = rm -f
RM_R = rm -rf

### Options
# -- BALSAIMPORTPATH defined by user
BALSACOPTS_COMPLETE = -b $(BALSAIMPORTPATH) $(BALSACOPTS)
BALSANETLISTOPTS = $(BALSAIMPORTPATH)

### Implementation technology

### User defined variables
BALSAIMPORTPATH = -I . 

### Suffix rules
.SUFFIXES: .balsa .breeze .ps

.breeze.ps:
	$(BREEZE2PS) $(BALSAIMPORTPATH) $*
.balsa.breeze:
	$(BALSAC) $(BALSACOPTS_COMPLETE) $*

all: test2.breeze test.breeze merga.breeze count16a.breeze buffer1a.breeze 
### Balsa rules

### Test harness rules
### Implementation rules
### Implementation simulations
### Miscellaneous rules
.PHONY: depend clean very-clean ps cost help

depend:
	balsa-make-makefile -b -p /home/yoyo/Desktop/LSV/project/balsa-4.0/test1126

clean:
	$(RM) buffer1a.breeze count16a.breeze merga.breeze test2.breeze test.breeze
	$(RM) buffer1a.ps count16a.ps merga.ps test2.ps test.ps

very-clean: clean
	$(RM_R) .libs

ps: test2.ps test.ps merga.ps count16a.ps buffer1a.ps

cost: test2.breeze test.breeze merga.breeze count16a.breeze buffer1a.breeze
	breeze-cost test2.breeze
	breeze-cost test.breeze
	breeze-cost merga.breeze
	breeze-cost count16a.breeze
	breeze-cost buffer1a.breeze

help:
	@echo 'Target              Meaning'
	@echo '------------------- ---------------------------------------------------'
	@echo 'clean               Remove auto-generated files (except test harnesses)'
	@echo 'cost                Estimate Breeze block costs'
	@echo 'ps                  Generate PostScript graphs for a Breeze files'
	@echo 'suffix rule .breeze Compile Balsa to Breeze'
	@echo 'suffix rule .ps     Generate a PostScript graph of a Breeze file'
	@echo 'very-clean          Remove all auto-generated files'
	@echo

