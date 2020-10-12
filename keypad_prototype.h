struct Keystroke {
  uint8_t type;
  uint8_t length;
  union {
    char* string;
    uint8_t 
  }
}
