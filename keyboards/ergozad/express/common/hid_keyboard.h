#ifndef HID_KEYBOARD_HEADER
#define HID_KEYBOARD_HEADER
#ifdef __cplusplus
// place here the simbol you wish to export to c programs too
extern "C" {
uint8_t     eeprom_read_byte(const uint8_t *Address);
void        eeprom_write_byte(uint8_t *Address, uint8_t Value);
void        eeprom_update_byte(uint8_t *Address, uint8_t Value);
uint16_t    eeprom_read_word(const uint16_t *Address);
void        eeprom_write_word(uint16_t *Address, uint16_t Value);
void        eeprom_update_word(uint16_t *Address, uint16_t Value);
uint32_t    eeprom_read_dword(const uint32_t *Address);
void        eeprom_write_dword(uint32_t *Address, uint32_t Value);
void        eeprom_update_dword(uint32_t *Address, uint32_t Value);
void        toggle_bluetooth();
const char *string_matrix_row(int layer, int row, int column);
void matrix_print(void);
void toggle_volt();
void print_prg_matrix();
};
#endif
#endif
