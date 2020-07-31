#ifndef GLOBAL_H_
#define GLOBAL_H_

/* ----------------------------------------------------------------------- */
/* ----------------------------  Project config -------------------------- */
/* ----------------------------------------------------------------------- */
/* Version 0.1 (24.01.2018)
 *
 * The project configuration needs to be completed for the project and
 * target platform used.
 */


/* ----------------------------------------------------------------------- */
/* ----------------------------  General config -------------------------- */
/* ----------------------------------------------------------------------- */

/* Unncomment if you want to get additional PRINTF outputs */
#define PRINTF

/* ----------------------------------------------------------------------- */
/* ------------------------------- pulseAT ------------------------------- */
/* ----------------------------------------------------------------------- */

/* Define which pulseAT project you're working on */

/* 1: Generic project setup, using SNMP infrastructure */
// #define pulseAT_generic

/* 2: DAEbot project, using DAEbot CAN infrastructure
 * More Infos here:
 * https://gitlab.idial.institute/pulseAT/pulseAT/
 * blob/master/documentation/pulseAT_at_DAEbot.md */
#define pulseAT_DAEbot

/* Define which operating system/ chip architecture
 * the target platform is using */

/* 1: Linux OS (working for ARM, x86 and x64) */
#define pulseAT_linux

/* 2: Arduino "OS", more includes like schedular needs to be done.
 * More Infos here: todo */
// #define pulseAT_Arduino

/* 3: STM32 "OS", more includes like schedular needs to be done.
 * More Infos here: todo */
// #define pulseAT_STM32

/* ----------------------------------------------------------------------- */
/* -------------------------------- DAEbot ------------------------------- */
/* ----------------------------------------------------------------------- */

/* Define which target SBC (single board computer) is used */

/* 1: Raspberry Pi */
#define rPI_DAEbot

/* 2: SMT32F3 Discovery */
// #define STM32F3_DAEbot

/* 3: Arduino Mega 2560 */
// #define ARDUINO2560_DAEbot

#endif /* GLOBAL_H_ */