cmake_minimum_required(VERSION 3.9)

project(tads2)

add_compile_definitions(ERR_LINK_MESSAGES USE_OS_LINEWRAP)

set (TADS_C_FILES
argize.c
ler.c 
mcm.c
mcs.c
mch.c
obj.c
cmd.c
errmsg.c
fioxor.c
oserr.c
runstat.c
fio.c
getstr.c
cmap.c
askf_os.c
indlg_os.c
osifc.c
dat.c
lst.c
run.c
out.c
voc.c
bif.c
suprun.c
regex.c
vocab.c
execmd.c
ply.c
qas.c
trd.c
dbgtr.c
linfdum.c
osrestad.c
bifgdum.c
output.c
osstzprs.c
osnoui.c
tadsrsc.c
)

add_library(tads2 STATIC
${TADS_C_FILES}
)

target_include_directories(tads2 PUBLIC
../os
)