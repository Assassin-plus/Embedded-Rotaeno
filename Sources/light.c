/*
 * light.c
 *
 *  Created on: May 26, 2022
 *      Author: dell
 */
//----------------------------------------------------------------
//created by luziqi
//turn on the light to express combo status
#include "derivative.h" 
void LIGHT_INIT(void){
	//clock start of PTC/PTB
	SIM_SCGC5|=(1<<10) | (1<<11);
	
	// Set PORTC as GPIO
	//light from left to right
	PORTC_PCR12=0x0100;//
	PORTC_PCR13=0x0100;//
	PORTC_PCR7 =0x0100;//
	PORTC_PCR6 =0x0100;//
	PORTB_PCR18=0x0100;//green controller
	PORTB_PCR19=0x0100;//blue controller
	PORTC_PCR9 =0x0100;//red controller
	
	// set pin of light to be i/o
	GPIOC_PDDR|=((1<<12)|(1<<13)|(1<<7)|(1<<6));
	GPIOC_PDOR|=(1<<12|1<<13|1<<7|1<<6);
	//set pin of colour to be i/o
	GPIOB_PDDR|=((1<<18)|(1<<19));
	GPIOB_PDOR&=~((1<<18)|(1<<19));
	GPIOC_PDDR|=(1<<9);
	GPIOC_PDOR&=~(1<<9);
}
void LIGHT_RESET(void){
	GPIOC_PDOR|=(1<<12|1<<13|1<<7|1<<6);
	GPIOB_PDOR&=~((1<<18)|(1<<19));
	GPIOC_PDOR&=~(1<<9);
}
void StatusDisplay(unsigned char status){
	LIGHT_RESET();
	switch(status){
	case 1: //perfect combo
		//LED1_group_en;
		GPIOC_PDOR&=~(1<<12);
		//LED_Blue_en;
		GPIOB_PDOR|=(1<<19);
		//LED_Red_en;
		GPIOC_PDOR|=(1<<9);
		break;
	case 2: //good combo
		//LED2_group_en;
		GPIOC_PDOR&=~(1<<13);
		//LED_Blue_en;
		GPIOB_PDOR|=(1<<19);
		break;
	case 3://bad combo
		//LED3_group_en;
		GPIOC_PDOR&=~(1<<7);
		//LED_Red_en;
		GPIOC_PDOR|=(1<<9);
		break;
	case 4: //miss combo
		//LED4_group_en;
		GPIOC_PDOR&=~(1<<6);
		//LED_Blue_en;
		GPIOB_PDOR|=(1<<19);
		//LED_Green_en;
		GPIOB_PDOR|=(1<<18);
		break;
	default://error
		//LED_group_en;
		GPIOC_PDOR&=~((1<<12)|(1<<13)|(1<<7)|(1<<6));
		//LED_Red_en;
		GPIOC_PDOR|=(1<<9);
		break;
	}
}

