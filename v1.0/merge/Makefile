
SHELL  = sh
REMOVE = rm -f

SolutionDir ?= ..

ACTIVE_BUILD=Release
XBOOT_HEX=$(SolutionDir)/avrXboot/xboot.hex
DRONE_HEX=$(SolutionDir)/Drone/$(ACTIVE_BUILD)/Drone.hex
MERGED_HEX=$(SolutionDir)/merge/mergedfile.hex

SREC_CAT=srec_cat

all: $(MERGED_HEX)

$(MERGED_HEX) : $(XBOOT_HEX) $(DRONE_HEX)
	$(SREC_CAT) $(XBOOT_HEX) -intel $(DRONE_HEX) -intel -o $(MERGED_HEX) -intel

clean:
	$(REMOVE) $(MERGED_HEX)
