#ifndef _OLED832_H_
#define _OLED832_H_

//RESET PIN
#define RESET_PIN						19

#define ABS(X)  ((X) > 0 ? (X) : -(X))

#define VGM_MIN_U8X                     0
#define VGM_PAGE_MAX                    6
#define VGM_COLUMN_MAX                  64

#define VGM_BRIGHTNESS                  0xFF

#define VGM_STATE_MIN                   0x00
#define VGM_STATE_MAX                   0xFF
#define VGM_STATE_55                    0x55
#define VGM_STATE_AA                    0xAA
#define VGM_STATE_FRAME_TOP             0x01
#define VGM_STATE_FRAME_BOTTOM          0x80

#define	OLED_ADDRESS 					(0x78>>1)
#define	OLED_OP_COMMAND					0x00
#define	OLED_OP_DATA 					0x40

#define SET_LOW_COL                     0x00
//#define SET_HIGH_COL(column)            (0x12 | (column))
#define SET_HIGH_COL                    0x12
#define SET_MEMORY_ADDRESSING_MODE      0x20
#define SET_COLUMN_ADDRESS              0x21
#define SET_PAGE_ADDRESS                0x22
#define SET_START_LINE(line)            (0x40 | (line))
#define SET_CONTRAST_CONTROL_FOR_BANK0  0x81
#define SET_CHARGE_PUMP_SETTING         0x8D
#define SET_SEGMENT_RE_MAP_COL0_SEG0    0xA0
#define SET_SEGMENT_RE_MAP_COL127_SEG0  0xA1
#define ENTIRE_DISPLAY_AND_GDDRAM_ON    0xA4
#define ENTIRE_DISPLAY_ON               0xA5
#define SET_NORMAL_DISPLAY              0xA6
#define SET_INVERSE_DISPLAY             0xA7
#define SET_MULTIPLEX_RATIO             0xA8
#define SET_DISPLAY_ON                  0xAF
#define SET_DISPLAY_OFF                 0xAE
#define SET_PAGE_START_ADDRESS          0xB0
#define SET_COM_OUTPUT_SCAN_UP          0xC0
#define SET_COM_OUTPUT_SCAN_DOWN        0xC8
#define SET_DISPLAY_OFFSET              0xD3
#define SET_DISPLAY_CLOCK_DIVIDE_RATIO  0xD5
#define SET_PRE_CHARGE_PERIOD           0xD9
#define SET_COM_PINS                    0xDA
#define SET_VCOMH_DESELECT_LEVEL        0xDB
#define SET_NOP                         0xE3

#define SCROLL_H_RIGHT                  0x26
#define SCROLL_H_LEFT                   0x27
#define CONTINUOUS_SCROLL_V_AND_H_RIGHT 0x29
#define CONTINUOUS_SCROLL_V_AND_H_LEFT  0x2A
#define DEACTIVATE_SCROLL               0x2E
#define ACTIVATE_SCROLL                 0x2F
#define SET_VERTICAL_SCROLL_AREA        0xA3

#define SET_FADE_BLINKING               0x23

#define SET_ZOOM_IN                     0xD6

#define VGM_LATENCY_1                   1
#define VGM_LATENCY_5                   5
#define VGM_LATENCY_50                  50
#define VGM_LATENCY_100                 100


typedef enum{
    VGM_MUX_16=16,
    VGM_MUX_17,
    VGM_MUX_18,
    VGM_MUX_19,
    VGM_MUX_20,
    VGM_MUX_21,
    VGM_MUX_22,
    VGM_MUX_23,
    VGM_MUX_24,
    VGM_MUX_25,
    VGM_MUX_26,
    VGM_MUX_27,
    VGM_MUX_28,
    VGM_MUX_29,
    VGM_MUX_30,
    VGM_MUX_31,
    VGM_MUX_32,
    VGM_MUX_33,
    VGM_MUX_34,
    VGM_MUX_35,
    VGM_MUX_36,
    VGM_MUX_37,
    VGM_MUX_38,
    VGM_MUX_39,
    VGM_MUX_40,
    VGM_MUX_41,
    VGM_MUX_42,
    VGM_MUX_43,
    VGM_MUX_44,
    VGM_MUX_45,
    VGM_MUX_46,
    VGM_MUX_47,
    VGM_MUX_48,
    VGM_MUX_49,
    VGM_MUX_50,
    VGM_MUX_51,
    VGM_MUX_52,
    VGM_MUX_53,
    VGM_MUX_54,
    VGM_MUX_55,
    VGM_MUX_56,
    VGM_MUX_57,
    VGM_MUX_58,
    VGM_MUX_59,
    VGM_MUX_60,
    VGM_MUX_61,
    VGM_MUX_62,
    VGM_MUX_63,
    VGM_MUX_64
}VGM_MUX_E;

typedef enum{
    DIRECT_UP=0,
    DIRECT_DOWN,
    DIRECT_RIGHT=0,
    DIRECT_LEFT
}DIRECT_SCROLL_E;

typedef enum{
    FADE_DISABLE,
    FADE_OUT=0x10,
    FADE_BLINKING=0x11
}FADE_MODE_E;

typedef enum{
    BLACK=0x00,
    WHITE=0xff
}VGM_COLOR_E;

typedef struct{
    uint8_t page_start;
    uint8_t page_end;
    uint8_t column_start;
    uint8_t column_end;
}gddram_pos_t;

typedef struct{
    uint8_t direct;
    uint8_t area_fixed;
    uint8_t area_scroll;
    uint8_t row;
    uint32_t interval;
}vgm_vroll_t;

typedef struct{
    uint8_t direct;
    uint8_t page_start;
    uint8_t page_end;
    uint8_t interval;
    uint32_t time_delay;
}vgm_hroll_t;

typedef struct{
    uint8_t direct;
    uint8_t area_fixed;
    uint8_t area_scroll;
    uint8_t page_start;
    uint8_t page_end;
    uint8_t interval;
    uint8_t offset;
		uint32_t time_delay;
}vgm_scroll_t;

void vgm_set_addr_mode(uint8_t addr_mode);
void vgm_set_page_address(uint8_t addr);
void vgm_set_column_address(uint8_t addr);
void vgm_set_pos(uint8_t vgm_page, uint8_t vgm_col);
void vgm_set_page_start_end(uint8_t page_start, uint8_t page_end);
void vgm_set_column_start_end(uint8_t col_start, uint8_t col_end);
void vgm_set_start_line(uint8_t addr);
uint8_t vgm_set_contrast(uint8_t contrast_level);
void vgm_set_charge_pump(uint8_t charge);
void vgm_set_segment_remap(uint8_t seg_remap);
void vgm_set_entire_display(uint8_t entire_disp);
void vgm_set_inverse_display(uint8_t disp);
void vgm_set_multiplex_ratio(uint8_t mux);
void vgm_set_display_on_off(uint8_t disp);
void vgm_set_common_remap(uint8_t com_remap);
void vgm_set_display_offset(uint8_t disp_off);
void vgm_set_display_clock(uint8_t disp_clock);
void vgm_set_precharge_period(uint8_t period);
void vgm_set_common_config(uint8_t conf);
void vgm_set_vcomh(uint8_t level);
void vgm_set_nop(void);
void vgm_gpio_init(void);
void vgm_reset(void);
void vgm_all_screen(uint8_t dat);
void vgm_fill_block(uint8_t dat, gddram_pos_t *vgm_pos);
void vgm_check_board();
void vgm_clear_screen(void);
void vgm_set_screen(void);
void vgm_show_row135(void);
void vgm_show_row246(void);
void vgm_show_column135(void);
void vgm_show_column246(void);
void vgm_show_frame();
void vgm_chess_board(void);
void vgm_show_font8x5(uint8_t start_page, uint8_t start_column, uint8_t ascii_ch);
void vgm_show_string8x5(uint8_t start_page, uint8_t start_column, const uint8_t *dat, const uint16_t str_len);
void vgm_vertical_scroll(vgm_vroll_t *v_scroll);
void vgm_horizontal_scroll(vgm_hroll_t *h_scroll);
void vgm_continuous_scroll(vgm_scroll_t *vh_scroll);
void vgm_deactivescroll();
void vgm_fade_in(void);
void vgm_fadeout_blinking(uint8_t mode, uint8_t interval);
void vgm_zoom_on_off(uint8_t zoom_in);
void vgm_init(void);
void oled832_init(void);


#endif // _OLED_H
