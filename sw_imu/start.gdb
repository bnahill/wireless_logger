define r
	mon reset halt
end

define l
	r
	load
	r
	set language c
end

file xnemo.elf
target remote :3333
monitor reset init

display/i $pc
