/*
	Author: Jesus Villanueva-Segovia
	Name: process_asm.h
	Purpose: Header file for process_asm functions so that I dont have to add them to kernel.c
*/
//--Prototypes -------------------------------------------------------------------------------------
void go(); //Prototype for our go function. This will go to our process_asm.S and head to our restore_branch label
void dispatch(); //Prototype for our dispatch function. This will dispatch a proccess. It will save the context of the running process and then it will then enqueue it to our ready queue.
//--------------------------------------------------------------------------------------------------