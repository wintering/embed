#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "nrf_gpio.h"
#include "comm.h"
#include "fonts.h"
#include "oled832.h"
#include "debug.h"

uint8_t g_vgm_mux;
uint8_t g_start_line;

static int8_t vgm_write_command(uint8_t command)
{
    return bus_write(OLED_ADDRESS, OLED_OP_COMMAND, &command, 1);
}

static int8_t vgm_write_data(uint8_t date)
{
	return bus_write(OLED_ADDRESS, OLED_OP_DATA, &date, 1);
}

static uint8_t vgm_read_data(void)
{
	return 0;
}

static uint8_t vgm_get_status(void)
{
	return 0;
}

/*********************************************************************************
Function:   	Set Addr Mode
Parameter:  	Default => Page Address Mode
*********************************************************************************/
void vgm_set_addr_mode(uint8_t addr_mode)
{
    vgm_write_command(SET_MEMORY_ADDRESSING_MODE);
    vgm_write_command(addr_mode);
}

/*********************************************************************************
Function:   	Page Address Mode Set Page Addr
Parameter:  	page: 48(bits)/8(bits/page)
            		(addr: 0-5)
*********************************************************************************/
void vgm_set_page_address(uint8_t addr)
{
	// Make sure that the address is 4 bits (only 8 pages)
	//addr &= 0x0F;
	vgm_write_command(SET_PAGE_START_ADDRESS | addr);
}

/*********************************************************************************
Function:   	Page Address Mode Set Column Addr
Parameter:  	column: 64(0 - 63)
            		(addr: 0-63)
*******************************************a**************************************/
void vgm_set_column_address(uint8_t addr)
{
	// Make sure the address is 7 bits
	//address &= 0x7F;
    //addr &= 0x3F;
	vgm_write_command(SET_LOW_COL + (addr & 0x0F));
	vgm_write_command(SET_HIGH_COL + (addr>>4));
}

/*********************************************************************************
Function:   	Page Address Mode Set Page and Column Addr
Parameter:  	page: 6(0-5); column: 64(0-63)
*********************************************************************************/
void vgm_set_pos(uint8_t vgm_page, uint8_t vgm_col)
{
    vgm_set_page_address(vgm_page);
    vgm_set_column_address(vgm_col);
}

/*********************************************************************************
Function:   	Horizontal or Vertical Address Mode Set Page Addr
Parameter: 	    page: 6(0-5)
*******************************************a**************************************/

void vgm_set_page_start_end(uint8_t page_start, uint8_t page_end)
{
	vgm_write_command(SET_PAGE_ADDRESS);			// Set Column Address
	vgm_write_command(page_start);			//   Default => 0x00 (Column Start Address)
	vgm_write_command(page_end);			//   Default => 0x7F (Column End Address)
}

/*********************************************************************************
Function:   	Horizontal or Vertical Address Mode Set Column Addr
Parameter:  	column: 64(0-63)
*******************************************a**************************************/
void vgm_set_column_start_end(uint8_t col_start, uint8_t col_end)
{
	vgm_write_command(SET_COLUMN_ADDRESS);			// Set Column Address
	vgm_write_command(col_start);			//   Default => 0x00 (Column Start Address)
	vgm_write_command(col_end);			//   Default => 0x7F (Column End Address)
}

/*********************************************************************************
Function:   	Set Display Start Line
Parameter:  	Default => 0x40 (0x00)
column:     	64(0-63)
*******************************************a**************************************/
void vgm_set_start_line(uint8_t addr)
{
	// Make sure address is 6 bits
	//address &= 0x3F;
	addr &= 0x3F;
	vgm_write_command(SET_START_LINE(addr));
}

/*********************************************************************************
Function:   	Set Contrast Control
Parameter:  	Default => 0x7F
contrast:   	0-255
*******************************************a**************************************/
uint8_t vgm_set_contrast(uint8_t contrast_level)
{
    vgm_write_command(SET_CONTRAST_CONTROL_FOR_BANK0);
	vgm_write_command(contrast_level);
	return contrast_level;
}

/*********************************************************************************
Parameter:  0x10=> Disable Charge Pump
            0x14=> Enable Charge Pump
            Default => 0x10
*********************************************************************************/
void vgm_set_charge_pump(uint8_t charge)
{
	vgm_write_command(SET_CHARGE_PUMP_SETTING );
	vgm_write_command(charge);
}

/*********************************************************************************
Function:   	Set Segment Re-Map
Parameter:  	0xA0 => Column Address 0 Mapped to SEG0
                0xA1 => Column Address 0 Mapped to SEG127
*********************************************************************************/
void vgm_set_segment_remap(uint8_t seg_remap)
{
	vgm_write_command(seg_remap);
}

/*********************************************************************************
Function:   	Set Entire Display On / Off
Parameter:  	0xA4 => Normal Display
                0xA5 => Entire Display On
            	Default => 0xA4
*********************************************************************************/
void vgm_set_entire_display(uint8_t entire_disp)
{
	vgm_write_command(entire_disp);
}

/*********************************************************************************
Function:   	Set Inverse Display On/Off
Parameter:  	0xA6 => Normal Display
                0xA7 => Inverse Display On
                Default => 0xA6
*********************************************************************************/
void vgm_set_inverse_display(uint8_t disp)
{
	vgm_write_command(disp);
}

/*********************************************************************************
Function:   	Set Multiplex Ratio
Parameter:
*********************************************************************************/
void vgm_set_multiplex_ratio(uint8_t mux)
{
	vgm_write_command(SET_MULTIPLEX_RATIO);			//
	vgm_write_command(mux);			//
}

/*********************************************************************************
Function:   	Set Display On/Off
Parameter:  	0xAE => Display Off
                0xAF => Display On
                Default => 0xAE
*********************************************************************************/
void vgm_set_display_on_off(uint8_t disp)
{
	vgm_write_command(disp);
}

/*********************************************************************************
Function:   	Set COM Output Scan Direction
Parameter:  	0xC0 => Scan from COM0 to 47
                0xC8 => Scan from COM47 to 0
                Default => 0xC0
*********************************************************************************/
void vgm_set_common_remap(uint8_t com_remap)
{
	vgm_write_command(com_remap);
}

/*********************************************************************************
Function:   	Set Display Offset
Parameter:   	Default => 0x00
*********************************************************************************/
void vgm_set_display_offset(uint8_t disp_off)
{
	vgm_write_command(SET_DISPLAY_OFFSET);
	vgm_write_command(disp_off);
}

/*********************************************************************************
Function:   	Set Display Clock Divide Ratio / Oscillator Frequency
Parameter:  	D[3:0] => Display Clock Divider
                D[7:4] => Oscillator Frequency
                Default => 0x80
*********************************************************************************/
void vgm_set_display_clock(uint8_t disp_clock)
{
	vgm_write_command(SET_DISPLAY_CLOCK_DIVIDE_RATIO);
	vgm_write_command(disp_clock);
}

/*********************************************************************************
Function:   	Set Pre-Charge Period
Parameter:  	D[3:0] => Phase 1 Period in 1~15 Display Clocks
                D[7:4] => Phase 2 Period in 1~15 Display Clocks
                Default => 0x22
                (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
*********************************************************************************/
void vgm_set_precharge_period(uint8_t period)
{
	vgm_write_command(SET_PRE_CHARGE_PERIOD);			
	vgm_write_command(period);			
}

/*********************************************************************************
Function:   	Set COM Pins Hardware Configuration
Parameter:  	Alternative COM Pin Configuration
                Disable COM Left/Right Re-Map
            	Default => 0x12
*********************************************************************************/
void vgm_set_common_config(uint8_t conf)
{
    vgm_write_command(SET_COM_PINS);
	vgm_write_command(conf);
}

/*********************************************************************************
Function:   	Set VCOMH Deselect Level
Parameter:  	Default => 0x20 (0.77*VCC)
*********************************************************************************/
void vgm_set_vcomh(uint8_t level)
{
	vgm_write_command(SET_VCOMH_DESELECT_LEVEL);
	vgm_write_command(level);
}

/*********************************************************************************
Function:   	Command for No Operation
Parameter:  	Default => 0x20 (0.77*VCC)
*********************************************************************************/
void vgm_set_nop(void)
{
	vgm_write_command(SET_NOP);
}

void vgm_gpio_init(void)
{
	nrf_gpio_cfg_output(RESET_PIN);
	nrf_gpio_pin_set(RESET_PIN);
}

void vgm_reset(void)
{
    nrf_delay_ms(10);
    nrf_gpio_pin_clear(RESET_PIN);
    nrf_delay_ms(VGM_LATENCY_50);
    nrf_gpio_pin_set(RESET_PIN);
    nrf_delay_ms(VGM_LATENCY_100);
}

//Page Address mode
void vgm_all_screen(uint8_t dat)
{
	uint8_t i, j;

	for(i=VGM_MIN_U8X; i<VGM_PAGE_MAX; i++)
	{
		vgm_set_pos(i, 0);
		for (j=VGM_MIN_U8X; j<VGM_COLUMN_MAX; j++)
		{
			vgm_write_data(dat);
		}
	}
}

void vgm_fill_block(uint8_t dat, gddram_pos_t *vgm_pos)
{
    uint8_t i, j;

    for(i=vgm_pos->page_start; i<(vgm_pos->page_end+1); i++)
    {
        //vgm_set_page_address(i);
        //vgm_set_column_address(vgm_pos->start_column);
        vgm_set_pos(i, vgm_pos->column_start);
        for(j=VGM_MIN_U8X; j<(vgm_pos->column_end-vgm_pos->column_start+1); j++)
        {
            vgm_write_data(dat);
        }
    }
}

void vgm_check_board()
{
    uint8_t i, j;

	for(i=VGM_MIN_U8X; i<VGM_PAGE_MAX; i++)
	{
		vgm_set_pos(i, 0);
		for (j=VGM_MIN_U8X; j<VGM_COLUMN_MAX/2; j++)
		{
			vgm_write_data(VGM_STATE_55);
			vgm_write_data(VGM_STATE_AA);
		}
	}
}

void vgm_clear_screen(void)
{
    vgm_all_screen(VGM_STATE_MIN);
}

void vgm_set_screen(void)
{
    vgm_all_screen(VGM_STATE_MAX);
}

void vgm_show_row135(void)
{
    vgm_all_screen(VGM_STATE_55);
}

void vgm_show_row246(void)
{
    vgm_all_screen(VGM_STATE_AA);
}

void vgm_show_column135(void)
{
    uint8_t i, j;

    for(i=VGM_MIN_U8X; i<VGM_PAGE_MAX; i++)
    {
        vgm_set_pos(i, 0);
        for(j=VGM_MIN_U8X; j<(VGM_COLUMN_MAX/2); j++)
        {
            vgm_write_data(VGM_STATE_MAX);
            vgm_write_data(VGM_STATE_MIN);
        }
    }
}

void vgm_show_column246(void)
{
    uint8_t i, j;

    for(i=VGM_MIN_U8X; i<VGM_PAGE_MAX; i++)
    {
        vgm_set_pos(i, 0);
        for(j=VGM_MIN_U8X; j<(VGM_COLUMN_MAX/2); j++)
        {
            vgm_write_data(VGM_STATE_MIN);
            vgm_write_data(VGM_STATE_MAX);
        }
    }
}

#if 1
void vgm_show_frame()
{
    uint8_t i,j;

    vgm_clear_screen();

	vgm_set_pos(0, 0);
	for(i=0; i<VGM_COLUMN_MAX; i++)
	{
		vgm_write_data(VGM_STATE_FRAME_TOP);
	}

	vgm_set_pos(5, 0);
	for(i=0; i<VGM_COLUMN_MAX; i++)
	{
		vgm_write_data(VGM_STATE_FRAME_BOTTOM);
	}

	for(i=0; i<8; i++)
	{
		vgm_set_page_address(i);

		for(j=0; j<VGM_COLUMN_MAX; j+=(VGM_COLUMN_MAX-1))
		{
			vgm_set_column_address(j);
			vgm_write_data(0xFF);
		}
	}
}
#endif

#if 0
void vgm_show_frame(void)
{
    uint8_t i,j;

    vgm_clear_screen();

	vgm_set_pos(0, 0);
    vgm_write_data(VGM_STATE_MAX);
	for(i=VGM_MIN_U8X; i<VGM_COLUMN_MAX-2; i++)
	{
		vgm_write_data(VGM_STATE_FRAME_TOP);
	}
    vgm_write_data(VGM_STATE_MAX);

    for(i=VGM_MIN_U8X+1; i<VGM_PAGE_MAX-1; i++)
    {
        vgm_set_pos(i, 0);
        vgm_write_data(VGM_STATE_MAX);
        for(j=VGM_MIN_U8X; j<(VGM_COLUMN_MAX-2); j++)
        {
            vgm_write_data(VGM_STATE_MIN);
        }
        vgm_write_data(VGM_STATE_MAX);
    }

    vgm_set_pos(5, 0);
    vgm_write_data(VGM_STATE_MAX);
    for(j=VGM_MIN_U8X; j<(VGM_COLUMN_MAX-2); j++)
    {
        vgm_write_data(VGM_STATE_FRAME_BOTTOM);
    }
    vgm_write_data(VGM_STATE_MAX);
}
#endif

/*********************************************************************************
Function:   	chess board
Parameter:
*********************************************************************************/
void vgm_chess_board(void)
{
   uint8_t page_number, column_number_1, column_number_2, board;
   board = 0xFF;
   //board = 0x00;

   for(page_number=VGM_MIN_U8X; page_number<VGM_PAGE_MAX; page_number++)
   {
	  vgm_set_pos(page_number, 0);
	  for(column_number_1=VGM_MIN_U8X; column_number_1<VGM_COLUMN_MAX/8; column_number_1++)
	  {
		 for(column_number_2=VGM_MIN_U8X; column_number_2<VGM_COLUMN_MAX/8; column_number_2++)
	     {
	   	    vgm_write_data(board);
		 }
		 //nrf_delay_ms(1000);
         board = ~board;
	  }
	  board = ~board;
   }
}

/*********************************************************************************
Function: 	Show Character (8x5)
Parameter:  start_page: Start Page
            start_column: Start Column
            ascii_ch: Ascii
            //basic_ext: Database ascii_basic or ascii_ext
*********************************************************************************/
void vgm_show_font8x5(uint8_t start_page, uint8_t start_column, uint8_t ascii_ch)
{
    uint8_t *ptr;
    uint8_t i;

    #if 0
	switch(basic_ext)
	{
		case 0:
			ptr = &ascii_basic[(ascii_ch-1)][0];
			break;
		case 1:
			ptr = &ascii_ext[(ascii_ch-1)][0];
			break;
	}
    #endif

    ptr = &ascii_basic[(ascii_ch-32-1)][0];

	vgm_set_pos(start_page, start_column);

    for(i=0; i<5; i++)
    {
        vgm_write_data(*ptr);
        ptr++;
    }
}

/*********************************************************************************
Function: 	Show Character (8x5)
Parameter:  start_page: Start Page
            start_column: Start Column
            dat: input string
            str_len: length of input string
*********************************************************************************/
void vgm_show_string8x5(uint8_t start_page, uint8_t start_column, const uint8_t *dat, const uint16_t str_len)
{
    uint8_t *ptr = NULL;
    uint8_t col = start_column;
    uint16_t i=0;

    ptr = (uint8_t *)dat;

    while((i++<str_len) && (*ptr!='\0'))
	{
		vgm_show_font8x5(start_page, col, *ptr);
		ptr++;
		col += 6;
	}
}

#if 0
/*********************************************************************************
Function: 	Show Character (8x8)
Parameter:  start_page: Start Page
            start_column: Start Column
            ascii_ch: Ascii
            //basic_ext: Database ascii_basic or ascii_ext
*********************************************************************************/
void vgm_show_font8x8(uint8_t start_page, uint8_t start_column, uint8_t ascii_ch)
{
    uint8_t *ptr;
    uint8_t i;

    //ptr = (uint8_t *)&ASCII8x8_Table[(ascii_ch-32)*8];
    ptr = (uint8_t *)&ASCII8x8_Table[(ascii_ch-32)*8+7];


	vgm_set_pos(start_page, start_column);

    for(i=0; i<8; i++)
    {
        vgm_write_data(*ptr);
        //ptr++;
        ptr--;
    }
}

/*********************************************************************************
Function: 	Show Character (8x8)
Parameter:  start_page: Start Page
            start_column: Start Column
            dat: input string
            str_len: length of input string
*********************************************************************************/
void vgm_show_string8x8(uint8_t start_page, uint8_t start_column, const uint8_t *dat, const uint16_t str_len)
{
    uint8_t *ptr = NULL;
    uint8_t col = start_column;
    uint16_t i=0;

    ptr = (uint8_t *)dat;

    while((i++<str_len) && (*ptr!='\0'))
	{
		vgm_show_font8x8(start_page, col, *ptr);
		ptr++;
		col += 8;
	}
}
#endif // 0

/*********************************************************************************
Function: 	vertical_scroll
Parameter:  struct{
            	uint8_t direct;         	//Scrolling Direction
            	uint8_t area_fixed;     	//Set Top Fixed Area, default 0x00
            	uint8_t area_scroll;    	//Set Vertical Scroll Area; default 0x40
            	uint8_t row;            	//Set Numbers of Row Scroll per Step
            	uint8_t interval;       	//Set Time Interval between Each Scroll Step
            }vgm_vert_scroll_t;

           	whole scroll: area_fixed = 0; area_scroll = g_vgm_mux;
            top scroll: area_fixed = 0; area_scroll < g_vgm_mux;
            central scroll: area_fixed + area_scroll < g_vgm_mux;
            bottom scroll: area_fixed + area_scroll = g_vgm_mux;

 whole scroll: 
	vgm_vroll_t v_scroll = {0, 0, 48, 8, 10}; //8: char occupy 8 lines
*********************************************************************************/
void vgm_vertical_scroll(vgm_vroll_t *v_scroll)
{
    uint8_t i;
	uint32_t j;

	if((v_scroll->area_fixed+v_scroll->area_scroll) > g_vgm_mux)
	{
		DEBUG_PRINT("vscroll fixed+scroll error\r\n");
		return;
	}

	if(g_start_line >= v_scroll->area_scroll)
	{
		DEBUG_PRINT("vscroll start_line error\r\n");
		return;
	}
		
    vgm_write_command(SET_VERTICAL_SCROLL_AREA);
    vgm_write_command(v_scroll->area_fixed);
    vgm_write_command(v_scroll->area_scroll);

    switch(v_scroll->direct)
    {
        //up
        case DIRECT_UP:
			for(i=0; i<v_scroll->area_scroll; i+=v_scroll->row)
			{
				vgm_set_start_line(i);
				g_start_line = i;
				for(j=0; j<v_scroll->interval; j++)
				{
					nrf_delay_ms(1);
				}
			}
			break;
        //down
		case DIRECT_DOWN:
			for(i=0; i<v_scroll->area_scroll; i+=v_scroll->row)
			{
				vgm_set_start_line(v_scroll->area_scroll-i);
				g_start_line = v_scroll->area_scroll-i;
				for(j=0; j<v_scroll->interval; j++)
				{
					nrf_delay_ms(1);
				}
			}
			break;
	}
	vgm_set_start_line(0x00);
	g_start_line = 0x00;
}

/*********************************************************************************
Function: 	Continuous Horizontal Scrolling (Partial or Full Screen)
Parameter:  struct{
             	uint8_t direct;        	//Scrolling Direction
             	uint8_t page_start;    	//Define Start Page Address
             	uint8_t page_end;      	//Define End Page Address
             	uint8_t interval;      	//Set Time Interval between Each Scroll Step
                    					//in Terms of Frame Frequency
             	uint8_t time_delay;    	//Delay Time
            }vgm_hroll_t;
interval:
	000	5 frames		001	64 frames  010	128 frames  011 256 frames
	100	3 frames		101	4 frames	 110	25 frames		111 2 frames
*********************************************************************************/
void vgm_horizontal_scroll(vgm_hroll_t *h_scroll)
{
    vgm_write_command(SCROLL_H_RIGHT | h_scroll->direct);
	vgm_write_command(0x00);
	vgm_write_command(h_scroll->page_start);
    vgm_write_command(h_scroll->interval);
	vgm_write_command(h_scroll->page_end);
    vgm_write_command(0x00);
    vgm_write_command(0xFF);
	vgm_write_command(ACTIVATE_SCROLL);
	nrf_delay_ms(h_scroll->time_delay);
}

/*********************************************************************************
Function: 	Continuous Vertical / Horizontal / Diagonal Scrolling
            (Partial or Full Screen)
Parameter:  struct{
                uint8_t direct;         //Scrolling Direction
                                        //"0x00" (Vertical & Rightward)
                                        //"0x01" (Vertical & Leftward)
                uint8_t area_fixed;
                uint8_t area_scroll;
                uint8_t page_start;
                uint8_t page_end;
                uint8_t interval;
                uint8_t offset;
            }vgm_scroll_t;
*********************************************************************************/
void vgm_continuous_scroll(vgm_scroll_t *vh_scroll)
{
	if(vh_scroll->offset >= vh_scroll->area_scroll)
	{
		DEBUG_PRINT("vh_scroll offset error\r\n");
		return;
	}
	
	vgm_write_command(SET_VERTICAL_SCROLL_AREA);			// Set Vertical Scroll Area
	vgm_write_command(vh_scroll->area_fixed);			//   Default => 0x00 (Top Fixed Area)
	vgm_write_command(vh_scroll->area_scroll);			//   Default => 0x40 (Vertical Scroll Area)

	vgm_write_command(CONTINUOUS_SCROLL_V_AND_H_RIGHT | vh_scroll->direct);			// Continuous Vertical & Horizontal Scroll Setup
	vgm_write_command(0x00);			//           => (Dummy Write for First Parameter)
	vgm_write_command(vh_scroll->page_start);
	vgm_write_command(vh_scroll->interval);
	vgm_write_command(vh_scroll->page_end);
	vgm_write_command(vh_scroll->offset);
	vgm_write_command(ACTIVATE_SCROLL);			// Activate Scrolling
	nrf_delay_ms(vh_scroll->time_delay);
}

void vgm_deactivescroll()
{
	vgm_write_command(DEACTIVATE_SCROLL);			// Deactivate Scrolling
}

/*********************************************************************************
Function: 	fade in
Parameter:
*********************************************************************************/
void vgm_fade_in(void)
{
    uint8_t i;

	vgm_set_display_on_off(SET_DISPLAY_ON);

	for(i=0; i<=(VGM_BRIGHTNESS); i++)
	{
		vgm_set_contrast(i);
		nrf_delay_us(200);
		nrf_delay_us(200);
		nrf_delay_us(200);
	}
}

#if 0
/*********************************************************************************
Function: 	fade out
Parameter:
*********************************************************************************/
void vgm_fade_out(void)
{
    uint8_t i;

	for(i=VGM_BRIGHTNESS; i>=0; i--)
	{
		vgm_set_contrast(i);
		nrf_delay_us(200);
		nrf_delay_us(200);
		nrf_delay_us(200);
	}
	vgm_set_display_on_off(SET_DISPLAY_OFF);
}
#endif

#if 1
/*********************************************************************************
Function: 	fade out, blinking
Parameter:
*********************************************************************************/
void vgm_fadeout_blinking(uint8_t mode, uint8_t interval)
{
    uint8_t i;

    vgm_write_command(SET_FADE_BLINKING);
    vgm_write_command(mode<<4 | interval);
}
#endif // 1

void vgm_zoom_on_off(uint8_t zoom_in)
{
    vgm_write_command(SET_ZOOM_IN);
    vgm_write_command(zoom_in);
}

void vgm_init(void)
{
	//Set Display Off
	vgm_set_display_on_off(SET_DISPLAY_OFF);   
	
	//Set Display Clock Divide Ratio/Oscillator Frequency
	//350KHz; divide: 1
	vgm_set_display_clock(0x80);

	//Set Pre-Charge Period; K
	//************0xF1\0x22
	vgm_set_precharge_period(0x22);
	
	//Set Contrast Controls; K
	//************0x9F
	vgm_set_contrast(0xCF);
	
	//Set Multiplex Ratio
	//48divide
	vgm_set_multiplex_ratio(0x2F);
	g_vgm_mux = 0x2F+1;

	//Set COM Pins Hardware Configuration
  	vgm_set_common_config(0x12) ;
	
	//Set Display Offset
	//RAM offset
	vgm_set_display_offset(0x00);

	//Set Display Start Line
	//ROW offset
	vgm_set_start_line(0x40);
	g_start_line = 0x00;

	//scan opposite
	vgm_set_common_remap(0xC8);

	//seg re-map
	vgm_set_segment_remap(0xA1);
	
	vgm_set_charge_pump(0x14);

	//Set VCOMH Deselect Level
	vgm_set_vcomh(0x00);

	//display RAM
	vgm_set_entire_display(0xA4);	

	//1: display on; 0: display off 
	vgm_set_inverse_display(0xA6);

	//vgm_set_display_on_off(SET_DISPLAY_ON);
}

void oled832_init(void)
{
	vgm_gpio_init();
	vgm_reset();
	vgm_init();
}


