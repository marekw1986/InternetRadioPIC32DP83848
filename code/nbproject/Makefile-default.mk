#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/code.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/code.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=btn/buttons.c btn/rotary.c config/config.c delay/delay.c fatfs/ff.c fatfs/ffsystem.c fatfs/ffunicode.c fatfs/SPIFlash.c fatfs/diskio.c fatfs/sd.c lcd/hd44780.c lcd/i2c.c net/Announce.c net/ARCFOUR.c net/ARP.c net/AutoIP.c net/BerkeleyAPI.c net/BigInt.c CustomHTTPApp.c net/DHCP.c net/DNS.c net/FTP.c net/GenericTCPServer.c net/Hashes.c net/Helpers.c net/HTTP2.c net/ICMP.c net/IP.c net/MPFS2.c MPFSImg2.c net/NBNS.c net/Random.c net/Reboot.c net/RSA.c net/SMTP.c net/SNTP.c net/SSL.c net/StackTsk.c net/TCP.c net/Telnet.c net/Tick.c net/UDP.c net/ZeroconfHelper.c net/ZeroconfLinkLocal.c net/ZeroconfMulticastDNS.c net/ETHPIC32ExtPhy.c net/ETHPIC32ExtPhyDP83848.c net/ETHPIC32IntMac.c nvram/nvram.c rc5/rc5.c time/time.c uart/uart.c usb/event.c usb/usb_config.c usb/usb_host.c usb/usb_host_msd.c usb/usb_host_msd_scsi.c vs1003/vs1003.c main.c common.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/btn/buttons.o ${OBJECTDIR}/btn/rotary.o ${OBJECTDIR}/config/config.o ${OBJECTDIR}/delay/delay.o ${OBJECTDIR}/fatfs/ff.o ${OBJECTDIR}/fatfs/ffsystem.o ${OBJECTDIR}/fatfs/ffunicode.o ${OBJECTDIR}/fatfs/SPIFlash.o ${OBJECTDIR}/fatfs/diskio.o ${OBJECTDIR}/fatfs/sd.o ${OBJECTDIR}/lcd/hd44780.o ${OBJECTDIR}/lcd/i2c.o ${OBJECTDIR}/net/Announce.o ${OBJECTDIR}/net/ARCFOUR.o ${OBJECTDIR}/net/ARP.o ${OBJECTDIR}/net/AutoIP.o ${OBJECTDIR}/net/BerkeleyAPI.o ${OBJECTDIR}/net/BigInt.o ${OBJECTDIR}/CustomHTTPApp.o ${OBJECTDIR}/net/DHCP.o ${OBJECTDIR}/net/DNS.o ${OBJECTDIR}/net/FTP.o ${OBJECTDIR}/net/GenericTCPServer.o ${OBJECTDIR}/net/Hashes.o ${OBJECTDIR}/net/Helpers.o ${OBJECTDIR}/net/HTTP2.o ${OBJECTDIR}/net/ICMP.o ${OBJECTDIR}/net/IP.o ${OBJECTDIR}/net/MPFS2.o ${OBJECTDIR}/MPFSImg2.o ${OBJECTDIR}/net/NBNS.o ${OBJECTDIR}/net/Random.o ${OBJECTDIR}/net/Reboot.o ${OBJECTDIR}/net/RSA.o ${OBJECTDIR}/net/SMTP.o ${OBJECTDIR}/net/SNTP.o ${OBJECTDIR}/net/SSL.o ${OBJECTDIR}/net/StackTsk.o ${OBJECTDIR}/net/TCP.o ${OBJECTDIR}/net/Telnet.o ${OBJECTDIR}/net/Tick.o ${OBJECTDIR}/net/UDP.o ${OBJECTDIR}/net/ZeroconfHelper.o ${OBJECTDIR}/net/ZeroconfLinkLocal.o ${OBJECTDIR}/net/ZeroconfMulticastDNS.o ${OBJECTDIR}/net/ETHPIC32ExtPhy.o ${OBJECTDIR}/net/ETHPIC32ExtPhyDP83848.o ${OBJECTDIR}/net/ETHPIC32IntMac.o ${OBJECTDIR}/nvram/nvram.o ${OBJECTDIR}/rc5/rc5.o ${OBJECTDIR}/time/time.o ${OBJECTDIR}/uart/uart.o ${OBJECTDIR}/usb/event.o ${OBJECTDIR}/usb/usb_config.o ${OBJECTDIR}/usb/usb_host.o ${OBJECTDIR}/usb/usb_host_msd.o ${OBJECTDIR}/usb/usb_host_msd_scsi.o ${OBJECTDIR}/vs1003/vs1003.o ${OBJECTDIR}/main.o ${OBJECTDIR}/common.o
POSSIBLE_DEPFILES=${OBJECTDIR}/btn/buttons.o.d ${OBJECTDIR}/btn/rotary.o.d ${OBJECTDIR}/config/config.o.d ${OBJECTDIR}/delay/delay.o.d ${OBJECTDIR}/fatfs/ff.o.d ${OBJECTDIR}/fatfs/ffsystem.o.d ${OBJECTDIR}/fatfs/ffunicode.o.d ${OBJECTDIR}/fatfs/SPIFlash.o.d ${OBJECTDIR}/fatfs/diskio.o.d ${OBJECTDIR}/fatfs/sd.o.d ${OBJECTDIR}/lcd/hd44780.o.d ${OBJECTDIR}/lcd/i2c.o.d ${OBJECTDIR}/net/Announce.o.d ${OBJECTDIR}/net/ARCFOUR.o.d ${OBJECTDIR}/net/ARP.o.d ${OBJECTDIR}/net/AutoIP.o.d ${OBJECTDIR}/net/BerkeleyAPI.o.d ${OBJECTDIR}/net/BigInt.o.d ${OBJECTDIR}/CustomHTTPApp.o.d ${OBJECTDIR}/net/DHCP.o.d ${OBJECTDIR}/net/DNS.o.d ${OBJECTDIR}/net/FTP.o.d ${OBJECTDIR}/net/GenericTCPServer.o.d ${OBJECTDIR}/net/Hashes.o.d ${OBJECTDIR}/net/Helpers.o.d ${OBJECTDIR}/net/HTTP2.o.d ${OBJECTDIR}/net/ICMP.o.d ${OBJECTDIR}/net/IP.o.d ${OBJECTDIR}/net/MPFS2.o.d ${OBJECTDIR}/MPFSImg2.o.d ${OBJECTDIR}/net/NBNS.o.d ${OBJECTDIR}/net/Random.o.d ${OBJECTDIR}/net/Reboot.o.d ${OBJECTDIR}/net/RSA.o.d ${OBJECTDIR}/net/SMTP.o.d ${OBJECTDIR}/net/SNTP.o.d ${OBJECTDIR}/net/SSL.o.d ${OBJECTDIR}/net/StackTsk.o.d ${OBJECTDIR}/net/TCP.o.d ${OBJECTDIR}/net/Telnet.o.d ${OBJECTDIR}/net/Tick.o.d ${OBJECTDIR}/net/UDP.o.d ${OBJECTDIR}/net/ZeroconfHelper.o.d ${OBJECTDIR}/net/ZeroconfLinkLocal.o.d ${OBJECTDIR}/net/ZeroconfMulticastDNS.o.d ${OBJECTDIR}/net/ETHPIC32ExtPhy.o.d ${OBJECTDIR}/net/ETHPIC32ExtPhyDP83848.o.d ${OBJECTDIR}/net/ETHPIC32IntMac.o.d ${OBJECTDIR}/nvram/nvram.o.d ${OBJECTDIR}/rc5/rc5.o.d ${OBJECTDIR}/time/time.o.d ${OBJECTDIR}/uart/uart.o.d ${OBJECTDIR}/usb/event.o.d ${OBJECTDIR}/usb/usb_config.o.d ${OBJECTDIR}/usb/usb_host.o.d ${OBJECTDIR}/usb/usb_host_msd.o.d ${OBJECTDIR}/usb/usb_host_msd_scsi.o.d ${OBJECTDIR}/vs1003/vs1003.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/common.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/btn/buttons.o ${OBJECTDIR}/btn/rotary.o ${OBJECTDIR}/config/config.o ${OBJECTDIR}/delay/delay.o ${OBJECTDIR}/fatfs/ff.o ${OBJECTDIR}/fatfs/ffsystem.o ${OBJECTDIR}/fatfs/ffunicode.o ${OBJECTDIR}/fatfs/SPIFlash.o ${OBJECTDIR}/fatfs/diskio.o ${OBJECTDIR}/fatfs/sd.o ${OBJECTDIR}/lcd/hd44780.o ${OBJECTDIR}/lcd/i2c.o ${OBJECTDIR}/net/Announce.o ${OBJECTDIR}/net/ARCFOUR.o ${OBJECTDIR}/net/ARP.o ${OBJECTDIR}/net/AutoIP.o ${OBJECTDIR}/net/BerkeleyAPI.o ${OBJECTDIR}/net/BigInt.o ${OBJECTDIR}/CustomHTTPApp.o ${OBJECTDIR}/net/DHCP.o ${OBJECTDIR}/net/DNS.o ${OBJECTDIR}/net/FTP.o ${OBJECTDIR}/net/GenericTCPServer.o ${OBJECTDIR}/net/Hashes.o ${OBJECTDIR}/net/Helpers.o ${OBJECTDIR}/net/HTTP2.o ${OBJECTDIR}/net/ICMP.o ${OBJECTDIR}/net/IP.o ${OBJECTDIR}/net/MPFS2.o ${OBJECTDIR}/MPFSImg2.o ${OBJECTDIR}/net/NBNS.o ${OBJECTDIR}/net/Random.o ${OBJECTDIR}/net/Reboot.o ${OBJECTDIR}/net/RSA.o ${OBJECTDIR}/net/SMTP.o ${OBJECTDIR}/net/SNTP.o ${OBJECTDIR}/net/SSL.o ${OBJECTDIR}/net/StackTsk.o ${OBJECTDIR}/net/TCP.o ${OBJECTDIR}/net/Telnet.o ${OBJECTDIR}/net/Tick.o ${OBJECTDIR}/net/UDP.o ${OBJECTDIR}/net/ZeroconfHelper.o ${OBJECTDIR}/net/ZeroconfLinkLocal.o ${OBJECTDIR}/net/ZeroconfMulticastDNS.o ${OBJECTDIR}/net/ETHPIC32ExtPhy.o ${OBJECTDIR}/net/ETHPIC32ExtPhyDP83848.o ${OBJECTDIR}/net/ETHPIC32IntMac.o ${OBJECTDIR}/nvram/nvram.o ${OBJECTDIR}/rc5/rc5.o ${OBJECTDIR}/time/time.o ${OBJECTDIR}/uart/uart.o ${OBJECTDIR}/usb/event.o ${OBJECTDIR}/usb/usb_config.o ${OBJECTDIR}/usb/usb_host.o ${OBJECTDIR}/usb/usb_host_msd.o ${OBJECTDIR}/usb/usb_host_msd_scsi.o ${OBJECTDIR}/vs1003/vs1003.o ${OBJECTDIR}/main.o ${OBJECTDIR}/common.o

# Source Files
SOURCEFILES=btn/buttons.c btn/rotary.c config/config.c delay/delay.c fatfs/ff.c fatfs/ffsystem.c fatfs/ffunicode.c fatfs/SPIFlash.c fatfs/diskio.c fatfs/sd.c lcd/hd44780.c lcd/i2c.c net/Announce.c net/ARCFOUR.c net/ARP.c net/AutoIP.c net/BerkeleyAPI.c net/BigInt.c CustomHTTPApp.c net/DHCP.c net/DNS.c net/FTP.c net/GenericTCPServer.c net/Hashes.c net/Helpers.c net/HTTP2.c net/ICMP.c net/IP.c net/MPFS2.c MPFSImg2.c net/NBNS.c net/Random.c net/Reboot.c net/RSA.c net/SMTP.c net/SNTP.c net/SSL.c net/StackTsk.c net/TCP.c net/Telnet.c net/Tick.c net/UDP.c net/ZeroconfHelper.c net/ZeroconfLinkLocal.c net/ZeroconfMulticastDNS.c net/ETHPIC32ExtPhy.c net/ETHPIC32ExtPhyDP83848.c net/ETHPIC32IntMac.c nvram/nvram.c rc5/rc5.c time/time.c uart/uart.c usb/event.c usb/usb_config.c usb/usb_host.c usb/usb_host_msd.c usb/usb_host_msd_scsi.c vs1003/vs1003.c main.c common.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/code.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX795F512L
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/btn/buttons.o: btn/buttons.c  .generated_files/flags/default/83a1a16a9274420c541f98b4b75c3cf391938861 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/btn" 
	@${RM} ${OBJECTDIR}/btn/buttons.o.d 
	@${RM} ${OBJECTDIR}/btn/buttons.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/btn/buttons.o.d" -o ${OBJECTDIR}/btn/buttons.o btn/buttons.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/btn/rotary.o: btn/rotary.c  .generated_files/flags/default/36cca71c7c7cd502a65ab996fc50d506ebead9a3 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/btn" 
	@${RM} ${OBJECTDIR}/btn/rotary.o.d 
	@${RM} ${OBJECTDIR}/btn/rotary.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/btn/rotary.o.d" -o ${OBJECTDIR}/btn/rotary.o btn/rotary.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/config/config.o: config/config.c  .generated_files/flags/default/91e1537a1e98300b5545dee6680ed09e2c6427b1 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/config" 
	@${RM} ${OBJECTDIR}/config/config.o.d 
	@${RM} ${OBJECTDIR}/config/config.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/config/config.o.d" -o ${OBJECTDIR}/config/config.o config/config.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/delay/delay.o: delay/delay.c  .generated_files/flags/default/ea74853c648e17fa2e95457a0a4ae34579b24ed4 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/delay" 
	@${RM} ${OBJECTDIR}/delay/delay.o.d 
	@${RM} ${OBJECTDIR}/delay/delay.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/delay/delay.o.d" -o ${OBJECTDIR}/delay/delay.o delay/delay.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/fatfs/ff.o: fatfs/ff.c  .generated_files/flags/default/a45341f4862e5eb39e02de18bd07d473f9e12c56 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/fatfs" 
	@${RM} ${OBJECTDIR}/fatfs/ff.o.d 
	@${RM} ${OBJECTDIR}/fatfs/ff.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/fatfs/ff.o.d" -o ${OBJECTDIR}/fatfs/ff.o fatfs/ff.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/fatfs/ffsystem.o: fatfs/ffsystem.c  .generated_files/flags/default/b40afc08f0dee8aa299ae45c7aaf22a1d58d8fe9 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/fatfs" 
	@${RM} ${OBJECTDIR}/fatfs/ffsystem.o.d 
	@${RM} ${OBJECTDIR}/fatfs/ffsystem.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/fatfs/ffsystem.o.d" -o ${OBJECTDIR}/fatfs/ffsystem.o fatfs/ffsystem.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/fatfs/ffunicode.o: fatfs/ffunicode.c  .generated_files/flags/default/982ed5fae0c8ef2b339e548f0ec80185c8ce09b5 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/fatfs" 
	@${RM} ${OBJECTDIR}/fatfs/ffunicode.o.d 
	@${RM} ${OBJECTDIR}/fatfs/ffunicode.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/fatfs/ffunicode.o.d" -o ${OBJECTDIR}/fatfs/ffunicode.o fatfs/ffunicode.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/fatfs/SPIFlash.o: fatfs/SPIFlash.c  .generated_files/flags/default/44ceea3467368847e2cff4729913d8346f7f80ad .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/fatfs" 
	@${RM} ${OBJECTDIR}/fatfs/SPIFlash.o.d 
	@${RM} ${OBJECTDIR}/fatfs/SPIFlash.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/fatfs/SPIFlash.o.d" -o ${OBJECTDIR}/fatfs/SPIFlash.o fatfs/SPIFlash.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/fatfs/diskio.o: fatfs/diskio.c  .generated_files/flags/default/a8c6ad2bfc543152a67768beeb40c0f7f3233b03 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/fatfs" 
	@${RM} ${OBJECTDIR}/fatfs/diskio.o.d 
	@${RM} ${OBJECTDIR}/fatfs/diskio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/fatfs/diskio.o.d" -o ${OBJECTDIR}/fatfs/diskio.o fatfs/diskio.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/fatfs/sd.o: fatfs/sd.c  .generated_files/flags/default/2426767f7c166e19b163b62f672d5dd88d887551 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/fatfs" 
	@${RM} ${OBJECTDIR}/fatfs/sd.o.d 
	@${RM} ${OBJECTDIR}/fatfs/sd.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/fatfs/sd.o.d" -o ${OBJECTDIR}/fatfs/sd.o fatfs/sd.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/lcd/hd44780.o: lcd/hd44780.c  .generated_files/flags/default/d7be5f42c80d0edf918c73140642df457eae1393 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/lcd" 
	@${RM} ${OBJECTDIR}/lcd/hd44780.o.d 
	@${RM} ${OBJECTDIR}/lcd/hd44780.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/lcd/hd44780.o.d" -o ${OBJECTDIR}/lcd/hd44780.o lcd/hd44780.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/lcd/i2c.o: lcd/i2c.c  .generated_files/flags/default/a7248ead0f481a4c77314f2bd1b73c0117396ca6 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/lcd" 
	@${RM} ${OBJECTDIR}/lcd/i2c.o.d 
	@${RM} ${OBJECTDIR}/lcd/i2c.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/lcd/i2c.o.d" -o ${OBJECTDIR}/lcd/i2c.o lcd/i2c.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/Announce.o: net/Announce.c  .generated_files/flags/default/86b40663a5e954eecd468189e617908394bba220 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/Announce.o.d 
	@${RM} ${OBJECTDIR}/net/Announce.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/Announce.o.d" -o ${OBJECTDIR}/net/Announce.o net/Announce.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/ARCFOUR.o: net/ARCFOUR.c  .generated_files/flags/default/1e0138126789e68191ccf73f682d3a09c5177771 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/ARCFOUR.o.d 
	@${RM} ${OBJECTDIR}/net/ARCFOUR.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/ARCFOUR.o.d" -o ${OBJECTDIR}/net/ARCFOUR.o net/ARCFOUR.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/ARP.o: net/ARP.c  .generated_files/flags/default/9dacd48e7b011d44ef9ca216840a7b2b7b52d7b3 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/ARP.o.d 
	@${RM} ${OBJECTDIR}/net/ARP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/ARP.o.d" -o ${OBJECTDIR}/net/ARP.o net/ARP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/AutoIP.o: net/AutoIP.c  .generated_files/flags/default/c36287e147fb1e54b1b08fd2d236558ea20e0995 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/AutoIP.o.d 
	@${RM} ${OBJECTDIR}/net/AutoIP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/AutoIP.o.d" -o ${OBJECTDIR}/net/AutoIP.o net/AutoIP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/BerkeleyAPI.o: net/BerkeleyAPI.c  .generated_files/flags/default/594ebf184b45cb14d1399860071648a871f83f7 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/BerkeleyAPI.o.d 
	@${RM} ${OBJECTDIR}/net/BerkeleyAPI.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/BerkeleyAPI.o.d" -o ${OBJECTDIR}/net/BerkeleyAPI.o net/BerkeleyAPI.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/BigInt.o: net/BigInt.c  .generated_files/flags/default/ccbbcc526cb28a8e8e3cb5a2a9de491222914857 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/BigInt.o.d 
	@${RM} ${OBJECTDIR}/net/BigInt.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/BigInt.o.d" -o ${OBJECTDIR}/net/BigInt.o net/BigInt.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/CustomHTTPApp.o: CustomHTTPApp.c  .generated_files/flags/default/105b2b615ebe3b03eb8360c8cea16b556b8317b2 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CustomHTTPApp.o.d 
	@${RM} ${OBJECTDIR}/CustomHTTPApp.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/CustomHTTPApp.o.d" -o ${OBJECTDIR}/CustomHTTPApp.o CustomHTTPApp.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/DHCP.o: net/DHCP.c  .generated_files/flags/default/a49183d7942b8ace520e1bcbe08bae9d68f40e62 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/DHCP.o.d 
	@${RM} ${OBJECTDIR}/net/DHCP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/DHCP.o.d" -o ${OBJECTDIR}/net/DHCP.o net/DHCP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/DNS.o: net/DNS.c  .generated_files/flags/default/24156efed99a5c38755ab4921f14bdd0908123f4 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/DNS.o.d 
	@${RM} ${OBJECTDIR}/net/DNS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/DNS.o.d" -o ${OBJECTDIR}/net/DNS.o net/DNS.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/FTP.o: net/FTP.c  .generated_files/flags/default/e38927b7e1c45fb803d5acc0b232f67840c1b9ba .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/FTP.o.d 
	@${RM} ${OBJECTDIR}/net/FTP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/FTP.o.d" -o ${OBJECTDIR}/net/FTP.o net/FTP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/GenericTCPServer.o: net/GenericTCPServer.c  .generated_files/flags/default/39a73dd93e7d5d9ab9b683a6e09c5a45ad4a761e .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/GenericTCPServer.o.d 
	@${RM} ${OBJECTDIR}/net/GenericTCPServer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/GenericTCPServer.o.d" -o ${OBJECTDIR}/net/GenericTCPServer.o net/GenericTCPServer.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/Hashes.o: net/Hashes.c  .generated_files/flags/default/780703fc5b7fff25e4da5cb45647d4b9a4cd68e3 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/Hashes.o.d 
	@${RM} ${OBJECTDIR}/net/Hashes.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/Hashes.o.d" -o ${OBJECTDIR}/net/Hashes.o net/Hashes.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/Helpers.o: net/Helpers.c  .generated_files/flags/default/1455dd51ee9f1143b69891467c2a838728d306a .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/Helpers.o.d 
	@${RM} ${OBJECTDIR}/net/Helpers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/Helpers.o.d" -o ${OBJECTDIR}/net/Helpers.o net/Helpers.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/HTTP2.o: net/HTTP2.c  .generated_files/flags/default/6d1b853b050cd19376e70faf7f6a0c31b838a9d .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/HTTP2.o.d 
	@${RM} ${OBJECTDIR}/net/HTTP2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/HTTP2.o.d" -o ${OBJECTDIR}/net/HTTP2.o net/HTTP2.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/ICMP.o: net/ICMP.c  .generated_files/flags/default/5db233d051b21afd2bdf7727dce29e04ce2243ce .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/ICMP.o.d 
	@${RM} ${OBJECTDIR}/net/ICMP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/ICMP.o.d" -o ${OBJECTDIR}/net/ICMP.o net/ICMP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/IP.o: net/IP.c  .generated_files/flags/default/7769665314707c1e7d476f70614d3ec13c2d6acc .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/IP.o.d 
	@${RM} ${OBJECTDIR}/net/IP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/IP.o.d" -o ${OBJECTDIR}/net/IP.o net/IP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/MPFS2.o: net/MPFS2.c  .generated_files/flags/default/e618d6e39e7b844c80568631317f95b703ce347e .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/MPFS2.o.d 
	@${RM} ${OBJECTDIR}/net/MPFS2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/MPFS2.o.d" -o ${OBJECTDIR}/net/MPFS2.o net/MPFS2.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/MPFSImg2.o: MPFSImg2.c  .generated_files/flags/default/3026e13d1551430b2a5fb975ebbc92367ac916ef .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MPFSImg2.o.d 
	@${RM} ${OBJECTDIR}/MPFSImg2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/MPFSImg2.o.d" -o ${OBJECTDIR}/MPFSImg2.o MPFSImg2.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/NBNS.o: net/NBNS.c  .generated_files/flags/default/9becf908f3a91faef998080421c239203cbeb814 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/NBNS.o.d 
	@${RM} ${OBJECTDIR}/net/NBNS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/NBNS.o.d" -o ${OBJECTDIR}/net/NBNS.o net/NBNS.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/Random.o: net/Random.c  .generated_files/flags/default/6c47fb49d3226c3410b9e3aa9386de54a7dac52e .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/Random.o.d 
	@${RM} ${OBJECTDIR}/net/Random.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/Random.o.d" -o ${OBJECTDIR}/net/Random.o net/Random.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/Reboot.o: net/Reboot.c  .generated_files/flags/default/4a60aac869218dcff5404db01c83093ae9925ab .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/Reboot.o.d 
	@${RM} ${OBJECTDIR}/net/Reboot.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/Reboot.o.d" -o ${OBJECTDIR}/net/Reboot.o net/Reboot.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/RSA.o: net/RSA.c  .generated_files/flags/default/8414b3c07e7632070aea991ead1cb8d72a992378 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/RSA.o.d 
	@${RM} ${OBJECTDIR}/net/RSA.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/RSA.o.d" -o ${OBJECTDIR}/net/RSA.o net/RSA.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/SMTP.o: net/SMTP.c  .generated_files/flags/default/2b5762346160e755dee2d08503daf786f1e3467a .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/SMTP.o.d 
	@${RM} ${OBJECTDIR}/net/SMTP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/SMTP.o.d" -o ${OBJECTDIR}/net/SMTP.o net/SMTP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/SNTP.o: net/SNTP.c  .generated_files/flags/default/f3adc56c116e966bf3d017067b1641225e7be3e2 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/SNTP.o.d 
	@${RM} ${OBJECTDIR}/net/SNTP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/SNTP.o.d" -o ${OBJECTDIR}/net/SNTP.o net/SNTP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/SSL.o: net/SSL.c  .generated_files/flags/default/966a94ca1c52fdd5a1780e2471b873ba776fd369 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/SSL.o.d 
	@${RM} ${OBJECTDIR}/net/SSL.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/SSL.o.d" -o ${OBJECTDIR}/net/SSL.o net/SSL.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/StackTsk.o: net/StackTsk.c  .generated_files/flags/default/1624acb3924a2bda959278c1e9337524dc28c740 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/StackTsk.o.d 
	@${RM} ${OBJECTDIR}/net/StackTsk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/StackTsk.o.d" -o ${OBJECTDIR}/net/StackTsk.o net/StackTsk.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/TCP.o: net/TCP.c  .generated_files/flags/default/1b98acd69062724ab499655b329b362e1c871975 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/TCP.o.d 
	@${RM} ${OBJECTDIR}/net/TCP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/TCP.o.d" -o ${OBJECTDIR}/net/TCP.o net/TCP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/Telnet.o: net/Telnet.c  .generated_files/flags/default/3f8b6aa752b0d73e5e8e68d615167f2585fc5fe5 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/Telnet.o.d 
	@${RM} ${OBJECTDIR}/net/Telnet.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/Telnet.o.d" -o ${OBJECTDIR}/net/Telnet.o net/Telnet.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/Tick.o: net/Tick.c  .generated_files/flags/default/5b5249c8ebec5f8e771be550999fdee0ab1f3d7a .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/Tick.o.d 
	@${RM} ${OBJECTDIR}/net/Tick.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/Tick.o.d" -o ${OBJECTDIR}/net/Tick.o net/Tick.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/UDP.o: net/UDP.c  .generated_files/flags/default/5bdeea7cfd6ff70c9498e6a5971da06427bd8da5 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/UDP.o.d 
	@${RM} ${OBJECTDIR}/net/UDP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/UDP.o.d" -o ${OBJECTDIR}/net/UDP.o net/UDP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/ZeroconfHelper.o: net/ZeroconfHelper.c  .generated_files/flags/default/2d589592b469d4da45d9b16f504c59f7e67af92f .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/ZeroconfHelper.o.d 
	@${RM} ${OBJECTDIR}/net/ZeroconfHelper.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/ZeroconfHelper.o.d" -o ${OBJECTDIR}/net/ZeroconfHelper.o net/ZeroconfHelper.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/ZeroconfLinkLocal.o: net/ZeroconfLinkLocal.c  .generated_files/flags/default/8efe444cc6a412fd48538cc22786a6ad8b57d6fd .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/ZeroconfLinkLocal.o.d 
	@${RM} ${OBJECTDIR}/net/ZeroconfLinkLocal.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/ZeroconfLinkLocal.o.d" -o ${OBJECTDIR}/net/ZeroconfLinkLocal.o net/ZeroconfLinkLocal.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/ZeroconfMulticastDNS.o: net/ZeroconfMulticastDNS.c  .generated_files/flags/default/5e6519f299609c447e772301645a8daf74ba8058 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/ZeroconfMulticastDNS.o.d 
	@${RM} ${OBJECTDIR}/net/ZeroconfMulticastDNS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/ZeroconfMulticastDNS.o.d" -o ${OBJECTDIR}/net/ZeroconfMulticastDNS.o net/ZeroconfMulticastDNS.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/ETHPIC32ExtPhy.o: net/ETHPIC32ExtPhy.c  .generated_files/flags/default/156fbf8a46131650feb021bd7dabdae4ab3b777f .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/ETHPIC32ExtPhy.o.d 
	@${RM} ${OBJECTDIR}/net/ETHPIC32ExtPhy.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/ETHPIC32ExtPhy.o.d" -o ${OBJECTDIR}/net/ETHPIC32ExtPhy.o net/ETHPIC32ExtPhy.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/ETHPIC32ExtPhyDP83848.o: net/ETHPIC32ExtPhyDP83848.c  .generated_files/flags/default/db0372982a79d7da447f40126197e16f24737d7d .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/ETHPIC32ExtPhyDP83848.o.d 
	@${RM} ${OBJECTDIR}/net/ETHPIC32ExtPhyDP83848.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/ETHPIC32ExtPhyDP83848.o.d" -o ${OBJECTDIR}/net/ETHPIC32ExtPhyDP83848.o net/ETHPIC32ExtPhyDP83848.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/ETHPIC32IntMac.o: net/ETHPIC32IntMac.c  .generated_files/flags/default/22d1169bb67ad705372dde233c26fe6f8af6c347 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/ETHPIC32IntMac.o.d 
	@${RM} ${OBJECTDIR}/net/ETHPIC32IntMac.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/ETHPIC32IntMac.o.d" -o ${OBJECTDIR}/net/ETHPIC32IntMac.o net/ETHPIC32IntMac.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/nvram/nvram.o: nvram/nvram.c  .generated_files/flags/default/1617a1dcf0c14cb4d27a932163133260bdd87fb3 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/nvram" 
	@${RM} ${OBJECTDIR}/nvram/nvram.o.d 
	@${RM} ${OBJECTDIR}/nvram/nvram.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/nvram/nvram.o.d" -o ${OBJECTDIR}/nvram/nvram.o nvram/nvram.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/rc5/rc5.o: rc5/rc5.c  .generated_files/flags/default/891cb867259a3dd75525635c71005c32fc9385cf .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/rc5" 
	@${RM} ${OBJECTDIR}/rc5/rc5.o.d 
	@${RM} ${OBJECTDIR}/rc5/rc5.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/rc5/rc5.o.d" -o ${OBJECTDIR}/rc5/rc5.o rc5/rc5.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/time/time.o: time/time.c  .generated_files/flags/default/b4b6093947dfaacf11f6a2e7793ac511d07d32eb .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/time" 
	@${RM} ${OBJECTDIR}/time/time.o.d 
	@${RM} ${OBJECTDIR}/time/time.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/time/time.o.d" -o ${OBJECTDIR}/time/time.o time/time.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/uart/uart.o: uart/uart.c  .generated_files/flags/default/a40fdd43e575b80015ee2fad5cafd9ee86825473 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/uart" 
	@${RM} ${OBJECTDIR}/uart/uart.o.d 
	@${RM} ${OBJECTDIR}/uart/uart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/uart/uart.o.d" -o ${OBJECTDIR}/uart/uart.o uart/uart.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/usb/event.o: usb/event.c  .generated_files/flags/default/a3d688199f9e04a242f134926f8c87a8aaf22877 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/event.o.d 
	@${RM} ${OBJECTDIR}/usb/event.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/usb/event.o.d" -o ${OBJECTDIR}/usb/event.o usb/event.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/usb/usb_config.o: usb/usb_config.c  .generated_files/flags/default/5492b21795d8b8c00e84c494264364ce8a7a2de2 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_config.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_config.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/usb/usb_config.o.d" -o ${OBJECTDIR}/usb/usb_config.o usb/usb_config.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/usb/usb_host.o: usb/usb_host.c  .generated_files/flags/default/e93587000dc4ace537a6968cdc10d744e806949e .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_host.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_host.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/usb/usb_host.o.d" -o ${OBJECTDIR}/usb/usb_host.o usb/usb_host.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/usb/usb_host_msd.o: usb/usb_host_msd.c  .generated_files/flags/default/9382e90de1a8e6a437dfa5cb0781f1c49b41923d .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_host_msd.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_host_msd.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/usb/usb_host_msd.o.d" -o ${OBJECTDIR}/usb/usb_host_msd.o usb/usb_host_msd.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/usb/usb_host_msd_scsi.o: usb/usb_host_msd_scsi.c  .generated_files/flags/default/8204e052554c426c415f860743c74fdf0cbb745e .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_host_msd_scsi.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_host_msd_scsi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/usb/usb_host_msd_scsi.o.d" -o ${OBJECTDIR}/usb/usb_host_msd_scsi.o usb/usb_host_msd_scsi.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/vs1003/vs1003.o: vs1003/vs1003.c  .generated_files/flags/default/8304789e7f46e1f5236b0366d5f5ae7fcc0aaa1d .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/vs1003" 
	@${RM} ${OBJECTDIR}/vs1003/vs1003.o.d 
	@${RM} ${OBJECTDIR}/vs1003/vs1003.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/vs1003/vs1003.o.d" -o ${OBJECTDIR}/vs1003/vs1003.o vs1003/vs1003.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/898cf5415939dea8bd47dff733feb7747d8e879c .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/common.o: common.c  .generated_files/flags/default/2569f97e78ae39bd9c5961d3c1b262f9445eca2e .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/common.o.d 
	@${RM} ${OBJECTDIR}/common.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/common.o.d" -o ${OBJECTDIR}/common.o common.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
else
${OBJECTDIR}/btn/buttons.o: btn/buttons.c  .generated_files/flags/default/d154dd2692c3c5de2a9e1c324fa1784c68403706 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/btn" 
	@${RM} ${OBJECTDIR}/btn/buttons.o.d 
	@${RM} ${OBJECTDIR}/btn/buttons.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/btn/buttons.o.d" -o ${OBJECTDIR}/btn/buttons.o btn/buttons.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/btn/rotary.o: btn/rotary.c  .generated_files/flags/default/420bdd1d4e682085a2b4b76ed53c73505eb924bc .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/btn" 
	@${RM} ${OBJECTDIR}/btn/rotary.o.d 
	@${RM} ${OBJECTDIR}/btn/rotary.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/btn/rotary.o.d" -o ${OBJECTDIR}/btn/rotary.o btn/rotary.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/config/config.o: config/config.c  .generated_files/flags/default/8a4d918d06fed9d3f7795d8e86f35dcf6e96e45a .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/config" 
	@${RM} ${OBJECTDIR}/config/config.o.d 
	@${RM} ${OBJECTDIR}/config/config.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/config/config.o.d" -o ${OBJECTDIR}/config/config.o config/config.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/delay/delay.o: delay/delay.c  .generated_files/flags/default/f0a753f32708001985703d22107d9ed4c40fc7d4 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/delay" 
	@${RM} ${OBJECTDIR}/delay/delay.o.d 
	@${RM} ${OBJECTDIR}/delay/delay.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/delay/delay.o.d" -o ${OBJECTDIR}/delay/delay.o delay/delay.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/fatfs/ff.o: fatfs/ff.c  .generated_files/flags/default/280cd81244e84b12e053eececb7f29ff21ac89f7 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/fatfs" 
	@${RM} ${OBJECTDIR}/fatfs/ff.o.d 
	@${RM} ${OBJECTDIR}/fatfs/ff.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/fatfs/ff.o.d" -o ${OBJECTDIR}/fatfs/ff.o fatfs/ff.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/fatfs/ffsystem.o: fatfs/ffsystem.c  .generated_files/flags/default/85ab1b7b8ca190a6b7824a3f8a1703bd81157e14 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/fatfs" 
	@${RM} ${OBJECTDIR}/fatfs/ffsystem.o.d 
	@${RM} ${OBJECTDIR}/fatfs/ffsystem.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/fatfs/ffsystem.o.d" -o ${OBJECTDIR}/fatfs/ffsystem.o fatfs/ffsystem.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/fatfs/ffunicode.o: fatfs/ffunicode.c  .generated_files/flags/default/2a27f6103b624ef195e6bb95b0158ac37ad64a94 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/fatfs" 
	@${RM} ${OBJECTDIR}/fatfs/ffunicode.o.d 
	@${RM} ${OBJECTDIR}/fatfs/ffunicode.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/fatfs/ffunicode.o.d" -o ${OBJECTDIR}/fatfs/ffunicode.o fatfs/ffunicode.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/fatfs/SPIFlash.o: fatfs/SPIFlash.c  .generated_files/flags/default/5557752bd57f2b7187132313377fc891440d5f7d .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/fatfs" 
	@${RM} ${OBJECTDIR}/fatfs/SPIFlash.o.d 
	@${RM} ${OBJECTDIR}/fatfs/SPIFlash.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/fatfs/SPIFlash.o.d" -o ${OBJECTDIR}/fatfs/SPIFlash.o fatfs/SPIFlash.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/fatfs/diskio.o: fatfs/diskio.c  .generated_files/flags/default/f7bf76903a24a74c2a92343e2f4e1f683c5c1ad2 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/fatfs" 
	@${RM} ${OBJECTDIR}/fatfs/diskio.o.d 
	@${RM} ${OBJECTDIR}/fatfs/diskio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/fatfs/diskio.o.d" -o ${OBJECTDIR}/fatfs/diskio.o fatfs/diskio.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/fatfs/sd.o: fatfs/sd.c  .generated_files/flags/default/280e15a7d028bf2689b028ff63e4755dd8daeea2 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/fatfs" 
	@${RM} ${OBJECTDIR}/fatfs/sd.o.d 
	@${RM} ${OBJECTDIR}/fatfs/sd.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/fatfs/sd.o.d" -o ${OBJECTDIR}/fatfs/sd.o fatfs/sd.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/lcd/hd44780.o: lcd/hd44780.c  .generated_files/flags/default/c129c653ea5f0cb774e03dda32f863b1b46c25ef .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/lcd" 
	@${RM} ${OBJECTDIR}/lcd/hd44780.o.d 
	@${RM} ${OBJECTDIR}/lcd/hd44780.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/lcd/hd44780.o.d" -o ${OBJECTDIR}/lcd/hd44780.o lcd/hd44780.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/lcd/i2c.o: lcd/i2c.c  .generated_files/flags/default/3453d5398e5dec7cca9fef67c861bce948da6000 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/lcd" 
	@${RM} ${OBJECTDIR}/lcd/i2c.o.d 
	@${RM} ${OBJECTDIR}/lcd/i2c.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/lcd/i2c.o.d" -o ${OBJECTDIR}/lcd/i2c.o lcd/i2c.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/Announce.o: net/Announce.c  .generated_files/flags/default/dea8026b55eb2f22a250d2d8721b432c959170a7 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/Announce.o.d 
	@${RM} ${OBJECTDIR}/net/Announce.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/Announce.o.d" -o ${OBJECTDIR}/net/Announce.o net/Announce.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/ARCFOUR.o: net/ARCFOUR.c  .generated_files/flags/default/2436743e6b7418db1fc8f94e284bf88961a0e1ba .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/ARCFOUR.o.d 
	@${RM} ${OBJECTDIR}/net/ARCFOUR.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/ARCFOUR.o.d" -o ${OBJECTDIR}/net/ARCFOUR.o net/ARCFOUR.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/ARP.o: net/ARP.c  .generated_files/flags/default/c80d5712bebc3b722737ec51cfdd57276f10b5cf .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/ARP.o.d 
	@${RM} ${OBJECTDIR}/net/ARP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/ARP.o.d" -o ${OBJECTDIR}/net/ARP.o net/ARP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/AutoIP.o: net/AutoIP.c  .generated_files/flags/default/5144940b7d0014da4e6f3364cbcdee9a97ca27d7 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/AutoIP.o.d 
	@${RM} ${OBJECTDIR}/net/AutoIP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/AutoIP.o.d" -o ${OBJECTDIR}/net/AutoIP.o net/AutoIP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/BerkeleyAPI.o: net/BerkeleyAPI.c  .generated_files/flags/default/dd8083dad44311239dd9ab6e20b379f17c58bafa .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/BerkeleyAPI.o.d 
	@${RM} ${OBJECTDIR}/net/BerkeleyAPI.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/BerkeleyAPI.o.d" -o ${OBJECTDIR}/net/BerkeleyAPI.o net/BerkeleyAPI.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/BigInt.o: net/BigInt.c  .generated_files/flags/default/ce0816a55b97f348795b8341310529d56ae22fb3 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/BigInt.o.d 
	@${RM} ${OBJECTDIR}/net/BigInt.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/BigInt.o.d" -o ${OBJECTDIR}/net/BigInt.o net/BigInt.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/CustomHTTPApp.o: CustomHTTPApp.c  .generated_files/flags/default/a6a8ea99da517a7bf6af3d1089d92c837f78b24 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CustomHTTPApp.o.d 
	@${RM} ${OBJECTDIR}/CustomHTTPApp.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/CustomHTTPApp.o.d" -o ${OBJECTDIR}/CustomHTTPApp.o CustomHTTPApp.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/DHCP.o: net/DHCP.c  .generated_files/flags/default/92c63d042d218fd136bf732329fb1698cd30643d .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/DHCP.o.d 
	@${RM} ${OBJECTDIR}/net/DHCP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/DHCP.o.d" -o ${OBJECTDIR}/net/DHCP.o net/DHCP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/DNS.o: net/DNS.c  .generated_files/flags/default/a56f55b86e033a347cbf3ac0c3e06c2dd6023600 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/DNS.o.d 
	@${RM} ${OBJECTDIR}/net/DNS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/DNS.o.d" -o ${OBJECTDIR}/net/DNS.o net/DNS.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/FTP.o: net/FTP.c  .generated_files/flags/default/8c50a1a5bf1bfbdbaf1088edd1096c29bd077aa0 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/FTP.o.d 
	@${RM} ${OBJECTDIR}/net/FTP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/FTP.o.d" -o ${OBJECTDIR}/net/FTP.o net/FTP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/GenericTCPServer.o: net/GenericTCPServer.c  .generated_files/flags/default/2c8ffc15ffc303ead45475cadb1d59cd0da701c9 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/GenericTCPServer.o.d 
	@${RM} ${OBJECTDIR}/net/GenericTCPServer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/GenericTCPServer.o.d" -o ${OBJECTDIR}/net/GenericTCPServer.o net/GenericTCPServer.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/Hashes.o: net/Hashes.c  .generated_files/flags/default/d4ccd24adf94e5619d2f20acc28c5da3d7d9880b .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/Hashes.o.d 
	@${RM} ${OBJECTDIR}/net/Hashes.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/Hashes.o.d" -o ${OBJECTDIR}/net/Hashes.o net/Hashes.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/Helpers.o: net/Helpers.c  .generated_files/flags/default/a0df6cf2701599bd343619f54bceee372f15ff0 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/Helpers.o.d 
	@${RM} ${OBJECTDIR}/net/Helpers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/Helpers.o.d" -o ${OBJECTDIR}/net/Helpers.o net/Helpers.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/HTTP2.o: net/HTTP2.c  .generated_files/flags/default/7a087d32f7625573c58498fdef946d1bca67634a .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/HTTP2.o.d 
	@${RM} ${OBJECTDIR}/net/HTTP2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/HTTP2.o.d" -o ${OBJECTDIR}/net/HTTP2.o net/HTTP2.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/ICMP.o: net/ICMP.c  .generated_files/flags/default/77b6dd432afd3c1673e71c4dbe9ba90831f51c29 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/ICMP.o.d 
	@${RM} ${OBJECTDIR}/net/ICMP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/ICMP.o.d" -o ${OBJECTDIR}/net/ICMP.o net/ICMP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/IP.o: net/IP.c  .generated_files/flags/default/600e45cf9854f6aa92ec858c9b2e89ae684b8ea1 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/IP.o.d 
	@${RM} ${OBJECTDIR}/net/IP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/IP.o.d" -o ${OBJECTDIR}/net/IP.o net/IP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/MPFS2.o: net/MPFS2.c  .generated_files/flags/default/faaf87d3250175a861310ac021479c82f03cd09e .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/MPFS2.o.d 
	@${RM} ${OBJECTDIR}/net/MPFS2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/MPFS2.o.d" -o ${OBJECTDIR}/net/MPFS2.o net/MPFS2.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/MPFSImg2.o: MPFSImg2.c  .generated_files/flags/default/53b6a31d2d725ff402e9cfabedfe859adcd74a53 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MPFSImg2.o.d 
	@${RM} ${OBJECTDIR}/MPFSImg2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/MPFSImg2.o.d" -o ${OBJECTDIR}/MPFSImg2.o MPFSImg2.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/NBNS.o: net/NBNS.c  .generated_files/flags/default/a3d15188d5f3de9fd4008b93058e656e9581507e .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/NBNS.o.d 
	@${RM} ${OBJECTDIR}/net/NBNS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/NBNS.o.d" -o ${OBJECTDIR}/net/NBNS.o net/NBNS.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/Random.o: net/Random.c  .generated_files/flags/default/578d7ee863bb6d3ce2355a03214c52c2bbb739b5 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/Random.o.d 
	@${RM} ${OBJECTDIR}/net/Random.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/Random.o.d" -o ${OBJECTDIR}/net/Random.o net/Random.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/Reboot.o: net/Reboot.c  .generated_files/flags/default/f429f488eaa543c72ad2160ad425241c4382027d .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/Reboot.o.d 
	@${RM} ${OBJECTDIR}/net/Reboot.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/Reboot.o.d" -o ${OBJECTDIR}/net/Reboot.o net/Reboot.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/RSA.o: net/RSA.c  .generated_files/flags/default/37542f782b18e8128b78a42b277da49b431c4bb9 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/RSA.o.d 
	@${RM} ${OBJECTDIR}/net/RSA.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/RSA.o.d" -o ${OBJECTDIR}/net/RSA.o net/RSA.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/SMTP.o: net/SMTP.c  .generated_files/flags/default/bc26aa8d072eb066c2a95bd9456c56008efea803 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/SMTP.o.d 
	@${RM} ${OBJECTDIR}/net/SMTP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/SMTP.o.d" -o ${OBJECTDIR}/net/SMTP.o net/SMTP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/SNTP.o: net/SNTP.c  .generated_files/flags/default/bbc2e618887cefb1bd6af94997efe7b78ddb2a0d .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/SNTP.o.d 
	@${RM} ${OBJECTDIR}/net/SNTP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/SNTP.o.d" -o ${OBJECTDIR}/net/SNTP.o net/SNTP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/SSL.o: net/SSL.c  .generated_files/flags/default/7b531793fe3b947f0dfd0b7f69075344b886bb3d .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/SSL.o.d 
	@${RM} ${OBJECTDIR}/net/SSL.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/SSL.o.d" -o ${OBJECTDIR}/net/SSL.o net/SSL.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/StackTsk.o: net/StackTsk.c  .generated_files/flags/default/9a649c1cf9c66c20b3019f334811dd26357b39b9 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/StackTsk.o.d 
	@${RM} ${OBJECTDIR}/net/StackTsk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/StackTsk.o.d" -o ${OBJECTDIR}/net/StackTsk.o net/StackTsk.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/TCP.o: net/TCP.c  .generated_files/flags/default/63983a70753aa77128e50d8991148cde30fed8fe .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/TCP.o.d 
	@${RM} ${OBJECTDIR}/net/TCP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/TCP.o.d" -o ${OBJECTDIR}/net/TCP.o net/TCP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/Telnet.o: net/Telnet.c  .generated_files/flags/default/11a4fff2ee46ca12975cbc5bb0ae7c8dc4f5c37a .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/Telnet.o.d 
	@${RM} ${OBJECTDIR}/net/Telnet.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/Telnet.o.d" -o ${OBJECTDIR}/net/Telnet.o net/Telnet.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/Tick.o: net/Tick.c  .generated_files/flags/default/9ae4365d512731dd0f1d3438420186b92b330e94 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/Tick.o.d 
	@${RM} ${OBJECTDIR}/net/Tick.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/Tick.o.d" -o ${OBJECTDIR}/net/Tick.o net/Tick.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/UDP.o: net/UDP.c  .generated_files/flags/default/60a2cc6162d756976818169ba61b94ccf9e92ca9 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/UDP.o.d 
	@${RM} ${OBJECTDIR}/net/UDP.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/UDP.o.d" -o ${OBJECTDIR}/net/UDP.o net/UDP.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/ZeroconfHelper.o: net/ZeroconfHelper.c  .generated_files/flags/default/b067688f0d6be5a0a3a44e85534c2ad69dd8acf4 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/ZeroconfHelper.o.d 
	@${RM} ${OBJECTDIR}/net/ZeroconfHelper.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/ZeroconfHelper.o.d" -o ${OBJECTDIR}/net/ZeroconfHelper.o net/ZeroconfHelper.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/ZeroconfLinkLocal.o: net/ZeroconfLinkLocal.c  .generated_files/flags/default/5f807f9f0d9052630b986471ddba69e07fa0382a .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/ZeroconfLinkLocal.o.d 
	@${RM} ${OBJECTDIR}/net/ZeroconfLinkLocal.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/ZeroconfLinkLocal.o.d" -o ${OBJECTDIR}/net/ZeroconfLinkLocal.o net/ZeroconfLinkLocal.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/ZeroconfMulticastDNS.o: net/ZeroconfMulticastDNS.c  .generated_files/flags/default/1ea3424553ae5cc9f72665afcd2de81eaec439f4 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/ZeroconfMulticastDNS.o.d 
	@${RM} ${OBJECTDIR}/net/ZeroconfMulticastDNS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/ZeroconfMulticastDNS.o.d" -o ${OBJECTDIR}/net/ZeroconfMulticastDNS.o net/ZeroconfMulticastDNS.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/ETHPIC32ExtPhy.o: net/ETHPIC32ExtPhy.c  .generated_files/flags/default/8f5676b15f6c16a2d6b69563e492b840a1536b4d .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/ETHPIC32ExtPhy.o.d 
	@${RM} ${OBJECTDIR}/net/ETHPIC32ExtPhy.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/ETHPIC32ExtPhy.o.d" -o ${OBJECTDIR}/net/ETHPIC32ExtPhy.o net/ETHPIC32ExtPhy.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/ETHPIC32ExtPhyDP83848.o: net/ETHPIC32ExtPhyDP83848.c  .generated_files/flags/default/8a5c7b204f2a96c8a5092b0007061d730a724265 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/ETHPIC32ExtPhyDP83848.o.d 
	@${RM} ${OBJECTDIR}/net/ETHPIC32ExtPhyDP83848.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/ETHPIC32ExtPhyDP83848.o.d" -o ${OBJECTDIR}/net/ETHPIC32ExtPhyDP83848.o net/ETHPIC32ExtPhyDP83848.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/net/ETHPIC32IntMac.o: net/ETHPIC32IntMac.c  .generated_files/flags/default/2dafaf9195a1af12c533d905af7ac2e8a9c7028b .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/net" 
	@${RM} ${OBJECTDIR}/net/ETHPIC32IntMac.o.d 
	@${RM} ${OBJECTDIR}/net/ETHPIC32IntMac.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/net/ETHPIC32IntMac.o.d" -o ${OBJECTDIR}/net/ETHPIC32IntMac.o net/ETHPIC32IntMac.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/nvram/nvram.o: nvram/nvram.c  .generated_files/flags/default/baf4f859602e664aa22ddb45c37257184f3cefe4 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/nvram" 
	@${RM} ${OBJECTDIR}/nvram/nvram.o.d 
	@${RM} ${OBJECTDIR}/nvram/nvram.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/nvram/nvram.o.d" -o ${OBJECTDIR}/nvram/nvram.o nvram/nvram.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/rc5/rc5.o: rc5/rc5.c  .generated_files/flags/default/dfbb5e7d81dd84fccfbb7f647fb8f4002ee5d6ee .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/rc5" 
	@${RM} ${OBJECTDIR}/rc5/rc5.o.d 
	@${RM} ${OBJECTDIR}/rc5/rc5.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/rc5/rc5.o.d" -o ${OBJECTDIR}/rc5/rc5.o rc5/rc5.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/time/time.o: time/time.c  .generated_files/flags/default/bc2d90a45aae203b685bfd935673ec985a9bdbbb .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/time" 
	@${RM} ${OBJECTDIR}/time/time.o.d 
	@${RM} ${OBJECTDIR}/time/time.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/time/time.o.d" -o ${OBJECTDIR}/time/time.o time/time.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/uart/uart.o: uart/uart.c  .generated_files/flags/default/a937de9b4a04fb583bd91567d02a3b72eabae195 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/uart" 
	@${RM} ${OBJECTDIR}/uart/uart.o.d 
	@${RM} ${OBJECTDIR}/uart/uart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/uart/uart.o.d" -o ${OBJECTDIR}/uart/uart.o uart/uart.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/usb/event.o: usb/event.c  .generated_files/flags/default/451cd534d94b4638283f2c28e8306086d7063f28 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/event.o.d 
	@${RM} ${OBJECTDIR}/usb/event.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/usb/event.o.d" -o ${OBJECTDIR}/usb/event.o usb/event.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/usb/usb_config.o: usb/usb_config.c  .generated_files/flags/default/d14f61246d50eb7b94efacda87d70dfd85cfcfe8 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_config.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_config.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/usb/usb_config.o.d" -o ${OBJECTDIR}/usb/usb_config.o usb/usb_config.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/usb/usb_host.o: usb/usb_host.c  .generated_files/flags/default/b746ca520b0c496136b55756657052e6b0f987 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_host.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_host.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/usb/usb_host.o.d" -o ${OBJECTDIR}/usb/usb_host.o usb/usb_host.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/usb/usb_host_msd.o: usb/usb_host_msd.c  .generated_files/flags/default/953b0e8b4f77b99096f720f74b22dc811fbe0c55 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_host_msd.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_host_msd.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/usb/usb_host_msd.o.d" -o ${OBJECTDIR}/usb/usb_host_msd.o usb/usb_host_msd.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/usb/usb_host_msd_scsi.o: usb/usb_host_msd_scsi.c  .generated_files/flags/default/96632bb7ae813eb97e5ba75ece38f7115db6e99b .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_host_msd_scsi.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_host_msd_scsi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/usb/usb_host_msd_scsi.o.d" -o ${OBJECTDIR}/usb/usb_host_msd_scsi.o usb/usb_host_msd_scsi.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/vs1003/vs1003.o: vs1003/vs1003.c  .generated_files/flags/default/1fc179b018fd8b75968f1cc09e3d71cd96ba852f .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}/vs1003" 
	@${RM} ${OBJECTDIR}/vs1003/vs1003.o.d 
	@${RM} ${OBJECTDIR}/vs1003/vs1003.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/vs1003/vs1003.o.d" -o ${OBJECTDIR}/vs1003/vs1003.o vs1003/vs1003.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/b965da68cc2fc1f35492c0a919a1105ab520e5b5 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
${OBJECTDIR}/common.o: common.c  .generated_files/flags/default/d8e8e20a603208d9e62ec34cca51813b39d78d98 .generated_files/flags/default/3a7ff55a6cb9e565cc82ee7521042155d62a01fd
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/common.o.d 
	@${RM} ${OBJECTDIR}/common.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_SUPPRESS_PLIB_WARNING -DSYSCLK=80000000L -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -MP -MMD -MF "${OBJECTDIR}/common.o.d" -o ${OBJECTDIR}/common.o common.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)    
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/code.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/code.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_heap_size=2048,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml 
	
else
${DISTDIR}/code.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/code.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=2048,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml 
	${MP_CC_DIR}/xc32-bin2hex ${DISTDIR}/code.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
