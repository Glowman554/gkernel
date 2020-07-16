import re
import os
import sys

frontend = "dialog"
configfile = "./kernel/include/config.h"

result = os.popen('which ' + frontend)
if result.read() == '':
	print("\nDas gewuenschte Frontend (" + frontend + ") steht nicht zur Verfuegung.")
	print("Sie muessen ein anderes waehlen (editieren sie config.py), oder das gewaehlte installieren.\n")
	sys.exit(1)

def show_dialog(options, define_name, define_value):
    title = "GKernel Konfiguration"
    print(define_value)
    if options['type'] == 'yesno':

        if define_value == "undef":
            default = " --defaultno"
        else:
            default = ""

        status = os.system(frontend + default + ' --title "' + title + '"  --yesno "' + options['desc'] + '" 0 0')
        if status == 0:
            return "#define " + define_name
        else:
            return "\n"
    if options['type'] == 'question':

        if define_value == "undef":
            default = " --defaultno"
        else:
            default = ""

        status = os.system(frontend + default + ' --title "' + title + '"  --yesno "' + options['desc'] + '" 0 0')
        if status == 0:
            return True
        else:
            return False
    elif options['type'] == 'text':
        result = os.popen(frontend + ' --stdout --title "' + title + '"  --inputbox "' + options['desc'] + '" 0 0 "' + define_value + '"').readline()
        
        return result, '#define ' + define_name + ' ' + result
    elif options['type'] == 'vendor':
        result = os.popen(frontend + ' --stdout --title "' + title + '"  --inputbox "' + options['desc'] + '" 0 0 "' + define_value + '"').readline()
        
        return result
    
    elif options['type'] == 'radio':
        opts = options['values'].split(',')
        
        opts_list = ''
        for opt in opts:
            opts_list = opts_list + ' "' + opt + '" "" '
            if opt == define_value:
                opts_list += 'on'
            else:
                opts_list += 'off'
            
        result = os.popen(frontend + ' --stdout --title "' + title + '"  --radiolist "' + options['desc'] + '" 0 0 ' + str(len(opts)) + '  ' + opts_list ).readline()
        return '#define ' + define_name + ' ' + result

syscalls = {"PUTC":"0","GETCHAR":"1","REBOOT":"2","PNUM":"3","READ_H":"4","READ_M":"5","READ_S":"6", "RTICK":"7", "GTICK":"8", "GETMOV":"9", \
            "CLRSCR":"10", "SETX":"11", "SETY":"12", "GPIXEL":"13", "SVGA":"14", "SPIXEL":"15", "LOADF":"17", "LS":"18", "LSPCI":"19", \
            "KVER":"20", "EXIT":"21", "KVEN":"22"}

config = "#ifndef CONFIG_H \n#define CONFIG_H \n"

f = open(".ver", "r")
ver = f.read()
f.close()

ver, tmp = show_dialog({"type":"text", "desc":"Version"}, "VERSION", ver)
config += tmp + "\n"

f = open(".ver", "w")
f.write(str(ver))
f.flush()
f.close()

f = open(".ven", "r")
ven = f.read()
f.close()

ven = show_dialog({"type":"vendor", "desc":"Vendor"},"",ven)

config += "#define VENDOR " + '"' + ven + '"\n'

f = open(".ven", "w")
f.write(str(ven))
f.flush()
f.close()

if show_dialog({"type":"question", "desc":"Configure Syscalls"}, "", "undef"):
    for i in syscalls:
        nul, tmp = show_dialog({"type":"text", "desc":i + " syscall"}, i, syscalls[i])
        config += tmp + "\n"
else:
    for i in syscalls:
        config += "#define " + i + " " + syscalls[i] + "\n"

config += "#endif"

if show_dialog({"type":"question", "desc":"Save Config"}, "", "undef"):
    f = open(configfile, "w")
    f.write(config)
    f.flush()
    f.close()
    print("configuration saved in " + configfile)
