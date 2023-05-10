/**
 * @file fsm_params.h
 * @brief Header for fsm_params.c file.
 * @author Jose Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
 * @date 04/05/2023
 */

#include <stdint.h>

// Wake-word = oye retina
static const uint8_t KEYWORD_ARRAY[] = {
    0xd3, 0x7b, 0xd8, 0x22, 0x47, 0x7d, 0x48, 0xd9, 0x1f, 0x13, 0x0e, 0x42, 0xd5, 0x97, 0x16, 0xfd, 0x2a, 0x89,
    0x23, 0x80, 0xaf, 0xef, 0x04, 0x43, 0x91, 0x21, 0x9b, 0x39, 0xaa, 0x73, 0x32, 0x83, 0xa8, 0x1b, 0x58, 0x90,
    0x32, 0x22, 0xf6, 0xa9, 0x69, 0xe4, 0x4c, 0x06, 0x17, 0xb9, 0x59, 0xc7, 0x63, 0x73, 0x96, 0xc5, 0xf1, 0x61,
    0x2f, 0x40, 0x53, 0x81, 0x60, 0x0e, 0x7d, 0x34, 0x59, 0xdc, 0x2c, 0xa9, 0x79, 0x98, 0x7d, 0x79, 0x71, 0x2f,
    0xf8, 0xfd, 0x7e, 0x96, 0xe7, 0xba, 0x3c, 0x8d, 0xdb, 0x43, 0xc1, 0xd3, 0x20, 0xa3, 0x49, 0x01, 0x97, 0x78,
    0x01, 0xf7, 0x09, 0xd7, 0x5c, 0xb8, 0x9c, 0x1e, 0x96, 0x77, 0x96, 0x91, 0xa4, 0x04, 0xd1, 0x60, 0x12, 0x33,
    0xcb, 0x4e, 0xaf, 0xda, 0xeb, 0x02, 0xfa, 0x6c, 0xb7, 0xc8, 0x13, 0x57, 0xe6, 0x98, 0x0d, 0x83, 0x7d, 0x74,
    0x7d, 0x89, 0xa7, 0xda, 0x70, 0xc3, 0xe5, 0x52, 0xb1, 0x96, 0xf6, 0xe0, 0x4f, 0xbb, 0x0e, 0xd1, 0xd6, 0x18,
    0x10, 0xba, 0x60, 0xfe, 0x32, 0xf5, 0x48, 0x57, 0x98, 0x6c, 0x52, 0xe1, 0x29, 0x98, 0xc7, 0x6c, 0x18, 0xdc,
    0x35, 0x89, 0x8a, 0xd4, 0x50, 0xa6, 0x2b, 0xc3, 0x5a, 0xd6, 0xbf, 0xb8, 0x01, 0x2a, 0x4a, 0x45, 0x48, 0x50,
    0xcf, 0x9c, 0x12, 0x19, 0x8d, 0xab, 0xf8, 0x58, 0xe3, 0x53, 0xa3, 0x7a, 0xd8, 0x43, 0xd8, 0x7f, 0xd9, 0x7e,
    0x90, 0xdc, 0x87, 0xd8, 0xe3, 0x97, 0x4e, 0xe7, 0x1e, 0x0b, 0x0a, 0x11, 0x38, 0x1b, 0xe2, 0xf3, 0xce, 0x82,
    0xee, 0xe9, 0x07, 0xc3, 0x6f, 0x9b, 0xcb, 0xc9, 0xfc, 0xed, 0x42, 0x4c, 0xec, 0x13, 0x98, 0x06, 0x2b, 0x46,
    0xc1, 0xd1, 0xbf, 0x04, 0xe6, 0x5a, 0xe8, 0x44, 0x01, 0xe7, 0x33, 0x99, 0x17, 0xd7, 0x18, 0x6a, 0x1f, 0x6c,
    0x26, 0x35, 0xf4, 0xd8, 0xad, 0xb8, 0x5e, 0xe8, 0x3e, 0xbf, 0xd4, 0xae, 0xf5, 0xc5, 0x4d, 0x11, 0x6a, 0xfb,
    0x75, 0xd6, 0x70, 0x17, 0xaf, 0x45, 0xe8, 0xbd, 0xa9, 0x4a, 0x87, 0xa4, 0x8f, 0x1f, 0x56, 0xcb, 0x96, 0x9d,
    0x4e, 0x9c, 0x96, 0x67, 0x21, 0x71, 0xa6, 0x34, 0x53, 0x7f, 0xae, 0xe1, 0x17, 0x6b, 0xd8, 0xb4, 0x45, 0x68,
    0xf1, 0xda, 0x64, 0xf0, 0x0d, 0x47, 0xed, 0x46, 0x86, 0xd4, 0x5f, 0x74, 0xd0, 0x87, 0x88, 0x33, 0xe3, 0x31,
    0xca, 0x24, 0x23, 0x25, 0x8b, 0x63, 0x33, 0x0e, 0x6b, 0xef, 0xbb, 0xd6, 0x1a, 0x6c, 0x40, 0x17, 0xf7, 0x2f,
    0x93, 0x7d, 0x60, 0x69, 0xf3, 0xd7, 0x23, 0x97, 0xc0, 0xf1, 0x07, 0x92, 0xbb, 0xeb, 0x6a, 0xb8, 0x72, 0x10,
    0x2d, 0x16, 0xb2, 0x68, 0x7c, 0xcd, 0x41, 0x89, 0x60, 0x71, 0x72, 0x91, 0x1e, 0xcb, 0x79, 0x2d, 0x90, 0xd9,
    0x28, 0x1e, 0x92, 0x9b, 0x6e, 0x2a, 0x5d, 0x0f, 0xef, 0x0e, 0xea, 0x25, 0xd3, 0x9a, 0x6f, 0xb6, 0x8f, 0x2e,
    0x32, 0x19, 0x3c, 0x0e, 0x52, 0x48, 0x40, 0xb1, 0xd1, 0xb1, 0xd9, 0x4a, 0x9c, 0x88, 0x38, 0x1f, 0x98, 0x3e,
    0x6a, 0x78, 0xe9, 0x7e, 0x91, 0xbe, 0x37, 0xfb, 0x75, 0x2d, 0x37, 0xc4, 0x8c, 0x26, 0x24, 0xc3, 0x4a, 0x05,
    0x7f, 0xe1, 0xb6, 0x20, 0x62, 0x9b, 0xec, 0xb7, 0xe2, 0xd9, 0xc6, 0x26, 0x3c, 0x4d, 0xeb, 0x53, 0x1d, 0x45,
    0x5b, 0x45, 0xd1, 0x45, 0xb9, 0xfb, 0xa8, 0x58, 0x22, 0x9f, 0x66, 0xf6, 0x4e, 0xbf, 0x12, 0x5c, 0xe3, 0x37,
    0x2e, 0x58, 0x7a, 0x4b, 0x17, 0x4a, 0x36, 0x4c, 0xec, 0x1b, 0x5f, 0x4c, 0xd6, 0x08, 0x24, 0x58, 0xa7, 0xfd,
    0x5b, 0x42, 0xd9, 0x9c, 0x1f, 0xae, 0x8c, 0xcf, 0xa8, 0x85, 0x7c, 0xeb, 0x1b, 0x5f, 0x01, 0xd6, 0xfe, 0xee,
    0x23, 0xe1, 0xee, 0xa8, 0x4c, 0x97, 0x16, 0x7e, 0x45, 0x89, 0xd9, 0xd5, 0x55, 0x00, 0xfe, 0xb2, 0xa3, 0x3d,
    0xd8, 0x8a, 0x8b, 0x4f, 0x15, 0xc1, 0x7d, 0x4a, 0xb9, 0xf2, 0xdd, 0x34, 0x1c, 0xe0, 0xf7, 0x80, 0xf7, 0xc1,
    0x61, 0xd1, 0xd5, 0x29, 0xdd, 0xe3, 0x8c, 0x5e, 0x01, 0xa8, 0x04, 0x95, 0x4a, 0x92, 0xc7, 0x5f, 0xa4, 0x5a,
    0x2d, 0x58, 0xfe, 0xce, 0x62, 0xc6, 0x1c, 0x77, 0x16, 0x8b, 0xfc, 0xec, 0xb2, 0x98, 0x30, 0x57, 0x01, 0x79,
    0x07, 0x30, 0xb9, 0xa7};

// context = iluminación_inteligente
static const uint8_t CONTEXT_ARRAY[] = {
    0x72, 0x68, 0x69, 0x6e, 0x6f, 0x32, 0x2e, 0x32, 0x2e, 0x30, 0x0c, 0x00, 0x00, 0x00, 0x27, 0x00, 0x69, 0x00,
    0x10, 0x51, 0x36, 0x33, 0x32, 0x38, 0x33, 0x32, 0x65, 0x73, 0x00, 0x14, 0x00, 0x00, 0x00, 0x47, 0x69, 0x74,
    0x48, 0x75, 0x62, 0x4f, 0x49, 0x44, 0x43, 0x5f, 0x31, 0x32, 0x37, 0x34, 0x34, 0x33, 0x39, 0x39, 0x32, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x34, 0x0c, 0x00, 0x00, 0x04, 0x00, 0x00,
    0x00, 0x6f, 0x6c, 0x6f, 0x63, 0x6f, 0x6e, 0x74, 0x65, 0x78, 0x74, 0x3a, 0x0a, 0x20, 0x20, 0x65, 0x78, 0x70,
    0x72, 0x65, 0x73, 0x73, 0x69, 0x6f, 0x6e, 0x73, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x63, 0x61, 0x6d, 0x62,
    0x69, 0x61, 0x72, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x68, 0x61,
    0x7a, 0x20, 0x71, 0x75, 0x65, 0x20, 0x6c, 0x61, 0x73, 0x20, 0x6c, 0x75, 0x63, 0x65, 0x73, 0x20, 0x73, 0x65,
    0x61, 0x6e, 0x20, 0x24, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x3a, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x0a, 0x20, 0x20,
    0x20, 0x20, 0x2d, 0x20, 0x63, 0x61, 0x6d, 0x62, 0x69, 0x61, 0x20, 0x6c, 0x61, 0x73, 0x20, 0x6c, 0x75, 0x63,
    0x65, 0x73, 0x20, 0x61, 0x20, 0x24, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x3a, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x0a,
    0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x68, 0x61, 0x7a, 0x20, 0x71, 0x75, 0x65, 0x20, 0x5b, 0x6c, 0x61, 0x2c,
    0x20, 0x6c, 0x61, 0x73, 0x5d, 0x20, 0x40, 0x6c, 0x75, 0x63, 0x65, 0x73, 0x20, 0x40, 0x64, 0x65, 0x6c, 0x20,
    0x24, 0x75, 0x62, 0x69, 0x63, 0x61, 0x63, 0x69, 0xc3, 0xb3, 0x6e, 0x3a, 0x75, 0x62, 0x69, 0x63, 0x61, 0x63,
    0x69, 0xc3, 0xb3, 0x6e, 0x20, 0x5b, 0x73, 0x65, 0x61, 0x2c, 0x20, 0x73, 0x65, 0x61, 0x6e, 0x5d, 0x20, 0x24,
    0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x3a, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20,
    0x68, 0x61, 0x7a, 0x20, 0x71, 0x75, 0x65, 0x20, 0x5b, 0x73, 0x65, 0x61, 0x2c, 0x20, 0x73, 0x65, 0x61, 0x6e,
    0x5d, 0x20, 0x24, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x3a, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x5b, 0x6c, 0x61,
    0x2c, 0x20, 0x6c, 0x61, 0x73, 0x5d, 0x20, 0x40, 0x6c, 0x75, 0x63, 0x65, 0x73, 0x20, 0x40, 0x64, 0x65, 0x6c,
    0x20, 0x24, 0x75, 0x62, 0x69, 0x63, 0x61, 0x63, 0x69, 0xc3, 0xb3, 0x6e, 0x3a, 0x75, 0x62, 0x69, 0x63, 0x61,
    0x63, 0x69, 0xc3, 0xb3, 0x6e, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x63, 0x61, 0x6d, 0x62, 0x69, 0x61,
    0x20, 0x5b, 0x6c, 0x61, 0x2c, 0x20, 0x6c, 0x61, 0x73, 0x5d, 0x20, 0x40, 0x6c, 0x75, 0x63, 0x65, 0x73, 0x20,
    0x40, 0x64, 0x65, 0x6c, 0x20, 0x24, 0x75, 0x62, 0x69, 0x63, 0x61, 0x63, 0x69, 0xc3, 0xb3, 0x6e, 0x3a, 0x75,
    0x62, 0x69, 0x63, 0x61, 0x63, 0x69, 0xc3, 0xb3, 0x6e, 0x20, 0x61, 0x20, 0x24, 0x63, 0x6f, 0x6c, 0x6f, 0x72,
    0x3a, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x63, 0x61, 0x6d, 0x62, 0x69,
    0x61, 0x20, 0x61, 0x20, 0x24, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x3a, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x5b,
    0x6c, 0x61, 0x2c, 0x20, 0x6c, 0x61, 0x73, 0x5d, 0x20, 0x40, 0x6c, 0x75, 0x63, 0x65, 0x73, 0x20, 0x40, 0x64,
    0x65, 0x6c, 0x20, 0x24, 0x75, 0x62, 0x69, 0x63, 0x61, 0x63, 0x69, 0xc3, 0xb3, 0x6e, 0x3a, 0x75, 0x62, 0x69,
    0x63, 0x61, 0x63, 0x69, 0xc3, 0xb3, 0x6e, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x63, 0x61, 0x6d, 0x62, 0x69, 0x61,
    0x72, 0x45, 0x73, 0x74, 0x61, 0x64, 0x6f, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x24, 0x65, 0x73,
    0x74, 0x61, 0x64, 0x6f, 0x3a, 0x65, 0x73, 0x74, 0x61, 0x64, 0x6f, 0x20, 0x28, 0x74, 0x6f, 0x64, 0x61, 0x73,
    0x29, 0x20, 0x6c, 0x61, 0x73, 0x20, 0x6c, 0x75, 0x63, 0x65, 0x73, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20,
    0x24, 0x65, 0x73, 0x74, 0x61, 0x64, 0x6f, 0x3a, 0x65, 0x73, 0x74, 0x61, 0x64, 0x6f, 0x20, 0x28, 0x6c, 0x61,
    0x2c, 0x20, 0x6c, 0x61, 0x73, 0x29, 0x20, 0x40, 0x6c, 0x75, 0x63, 0x65, 0x73, 0x20, 0x40, 0x64, 0x65, 0x6c,
    0x20, 0x24, 0x75, 0x62, 0x69, 0x63, 0x61, 0x63, 0x69, 0xc3, 0xb3, 0x6e, 0x3a, 0x75, 0x62, 0x69, 0x63, 0x61,
    0x63, 0x69, 0xc3, 0xb3, 0x6e, 0x0a, 0x20, 0x20, 0x73, 0x6c, 0x6f, 0x74, 0x73, 0x3a, 0x0a, 0x20, 0x20, 0x20,
    0x20, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x61, 0x7a, 0x75, 0x6c,
    0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x76, 0x65, 0x72, 0x64, 0x65, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d,
    0x20, 0x72, 0x6f, 0x73, 0x61, 0x64, 0x6f, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x6d, 0x6f, 0x72, 0x61,
    0x64, 0x6f, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x72, 0x6f, 0x6a, 0x6f, 0x0a, 0x20, 0x20, 0x20, 0x20,
    0x2d, 0x20, 0x62, 0x6c, 0x61, 0x6e, 0x63, 0x6f, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x61, 0x6d, 0x61,
    0x72, 0x69, 0x6c, 0x6c, 0x6f, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x75, 0x62, 0x69, 0x63, 0x61, 0x63, 0x69, 0xc3,
    0xb3, 0x6e, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x62, 0x61, 0xc3, 0xb1, 0x6f, 0x0a, 0x20, 0x20,
    0x20, 0x20, 0x2d, 0x20, 0x61, 0x72, 0x6d, 0x61, 0x72, 0x69, 0x6f, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20,
    0x63, 0x6f, 0x63, 0x69, 0x6e, 0x61, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x73, 0x61, 0x6c, 0x61, 0x0a,
    0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x64, 0x65, 0x73, 0x70, 0x65, 0x6e, 0x73, 0x61, 0x0a, 0x20, 0x20, 0x20,
    0x20, 0x2d, 0x20, 0x64, 0x6f, 0x72, 0x6d, 0x69, 0x74, 0x6f, 0x72, 0x69, 0x6f, 0x0a, 0x20, 0x20, 0x20, 0x20,
    0x2d, 0x20, 0x68, 0x61, 0x62, 0x69, 0x74, 0x61, 0x63, 0x69, 0xc3, 0xb3, 0x6e, 0x0a, 0x20, 0x20, 0x20, 0x20,
    0x2d, 0x20, 0x70, 0x61, 0x73, 0x69, 0x6c, 0x6c, 0x6f, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x65, 0x73, 0x74, 0x61,
    0x64, 0x6f, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x65, 0x6e, 0x63, 0x69, 0x65, 0x6e, 0x64, 0x61,
    0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x61, 0x70, 0x61, 0x67, 0x61, 0x0a, 0x20, 0x20, 0x6d, 0x61, 0x63,
    0x72, 0x6f, 0x73, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x6c, 0x75, 0x63, 0x65, 0x73, 0x3a, 0x0a, 0x20, 0x20,
    0x20, 0x20, 0x2d, 0x20, 0x6c, 0x75, 0x63, 0x65, 0x73, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x6c, 0x75,
    0x7a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x64, 0x65, 0x6c, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x64,
    0x65, 0x6c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x64, 0x65, 0x20, 0x6c, 0x61, 0x0a, 0x20, 0x20, 0x20,
    0x20, 0x2d, 0x20, 0x65, 0x6e, 0x20, 0x65, 0x6c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x65, 0x6e, 0x20,
    0x6c, 0x61, 0x0a, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00,
    0x2b, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x39, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x43, 0x00,
    0x00, 0x00, 0x4c, 0x00, 0x00, 0x00, 0x57, 0x00, 0x00, 0x00, 0x5d, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00,
    0x6c, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x83, 0x00,
    0x00, 0x00, 0x89, 0x00, 0x00, 0x00, 0x8d, 0x00, 0x00, 0x00, 0x94, 0x00, 0x00, 0x00, 0x9c, 0x00, 0x00, 0x00,
    0xa0, 0x00, 0x00, 0x00, 0xa5, 0x00, 0x00, 0x00, 0xac, 0x00, 0x00, 0x00, 0xb1, 0x00, 0x00, 0x00, 0xb6, 0x00,
    0x00, 0x00, 0xba, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xc6, 0x00, 0x00, 0x00, 0x61, 0x6d, 0x61, 0x72,
    0x69, 0x6c, 0x6c, 0x6f, 0x00, 0x61, 0x70, 0x61, 0x67, 0x61, 0x00, 0x61, 0x72, 0x6d, 0x61, 0x72, 0x69, 0x6f,
    0x00, 0x61, 0x7a, 0x75, 0x6c, 0x00, 0x61, 0x00, 0x62, 0x61, 0xc3, 0xb1, 0x6f, 0x00, 0x62, 0x6c, 0x61, 0x6e,
    0x63, 0x6f, 0x00, 0x63, 0x61, 0x6d, 0x62, 0x69, 0x61, 0x00, 0x63, 0x6f, 0x63, 0x69, 0x6e, 0x61, 0x00, 0x64,
    0x65, 0x20, 0x6c, 0x61, 0x00, 0x64, 0x65, 0x6c, 0x00, 0x64, 0x65, 0x73, 0x70, 0x65, 0x6e, 0x73, 0x61, 0x00,
    0x64, 0x6f, 0x72, 0x6d, 0x69, 0x74, 0x6f, 0x72, 0x69, 0x6f, 0x00, 0x65, 0x6e, 0x20, 0x65, 0x6c, 0x00, 0x65,
    0x6e, 0x20, 0x6c, 0x61, 0x00, 0x65, 0x6e, 0x63, 0x69, 0x65, 0x6e, 0x64, 0x61, 0x00, 0x68, 0x61, 0x62, 0x69,
    0x74, 0x61, 0x63, 0x69, 0xc3, 0xb3, 0x6e, 0x00, 0x68, 0x61, 0x7a, 0x00, 0x6c, 0x61, 0x73, 0x00, 0x6c, 0x61,
    0x00, 0x6c, 0x75, 0x63, 0x65, 0x73, 0x00, 0x6c, 0x75, 0x7a, 0x00, 0x6d, 0x6f, 0x72, 0x61, 0x64, 0x6f, 0x00,
    0x70, 0x61, 0x73, 0x69, 0x6c, 0x6c, 0x6f, 0x00, 0x71, 0x75, 0x65, 0x00, 0x72, 0x6f, 0x6a, 0x6f, 0x00, 0x72,
    0x6f, 0x73, 0x61, 0x64, 0x6f, 0x00, 0x73, 0x61, 0x6c, 0x61, 0x00, 0x73, 0x65, 0x61, 0x6e, 0x00, 0x73, 0x65,
    0x61, 0x00, 0x74, 0x6f, 0x64, 0x61, 0x73, 0x00, 0x76, 0x65, 0x72, 0x64, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x09, 0x00,
    0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00,
    0x0e, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x12, 0x00,
    0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00,
    0x17, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x1b, 0x00,
    0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x13, 0x00,
    0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00,
    0x28, 0x00, 0x00, 0x00, 0x2e, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00, 0x00, 0x3d, 0x00,
    0x00, 0x00, 0x47, 0x00, 0x00, 0x00, 0x4b, 0x00, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x57, 0x00, 0x00, 0x00,
    0x60, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00, 0x67, 0x00, 0x00, 0x00, 0x6c, 0x00,
    0x00, 0x00, 0x6f, 0x00, 0x00, 0x00, 0x75, 0x00, 0x00, 0x00, 0x7b, 0x00, 0x00, 0x00, 0x7d, 0x00, 0x00, 0x00,
    0x81, 0x00, 0x00, 0x00, 0x87, 0x00, 0x00, 0x00, 0x8b, 0x00, 0x00, 0x00, 0x8f, 0x00, 0x00, 0x00, 0x92, 0x00,
    0x00, 0x00, 0x97, 0x00, 0x00, 0x00, 0x9c, 0x00, 0x00, 0x00, 0x01, 0x0b, 0x01, 0x1b, 0x07, 0x1c, 0x0d, 0x01,
    0x0e, 0x01, 0x18, 0x01, 0x01, 0x1b, 0x0b, 0x01, 0x1b, 0x08, 0x0d, 0x01, 0x20, 0x12, 0x0a, 0x01, 0x02, 0x01,
    0x1a, 0x0d, 0x02, 0x0a, 0x01, 0x17, 0x09, 0x0d, 0x09, 0x01, 0x0b, 0x02, 0x08, 0x01, 0x09, 0x0d, 0x20, 0x07,
    0x0c, 0x01, 0x03, 0x04, 0x0a, 0x01, 0x03, 0x04, 0x0a, 0x03, 0x04, 0x10, 0x0e, 0x04, 0x0c, 0x10, 0x01, 0x03,
    0x0d, 0x1b, 0x0b, 0x07, 0x11, 0x0d, 0x1b, 0x08, 0x0d, 0x04, 0x0c, 0x04, 0x0a, 0x04, 0x0c, 0x0a, 0x01, 0x04,
    0x0c, 0x20, 0x08, 0x04, 0x0c, 0x03, 0x01, 0x01, 0x1f, 0x07, 0x11, 0x01, 0x20, 0x08, 0x0d, 0x0c, 0x01, 0x20,
    0x0a, 0x01, 0x10, 0x0a, 0x01, 0x0a, 0x12, 0x20, 0x04, 0x10, 0x0a, 0x12, 0x20, 0x0b, 0x0d, 0x1b, 0x01, 0x16,
    0x0d, 0x0e, 0x01, 0x10, 0x07, 0x1c, 0x0d, 0x09, 0x04, 0x0f, 0x0d, 0x14, 0x0d, 0x0f, 0x0d, 0x10, 0x01, 0x16,
    0x0d, 0x10, 0x01, 0x0a, 0x01, 0x10, 0x04, 0x01, 0x0c, 0x10, 0x04, 0x01, 0x11, 0x0d, 0x16, 0x01, 0x10, 0x02,
    0x04, 0x1b, 0x16, 0x04, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x09, 0x00,
    0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00,
    0x15, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x19, 0x00,
    0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x19, 0x00,
    0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x0c, 0x00,
    0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x07, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x18, 0x00,
    0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00,
    0x0d, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x14, 0x00,
    0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x1a, 0x00,
    0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00,
    0x1f, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x23, 0x00,
    0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x00, 0x65, 0x73, 0x74, 0x61, 0x64, 0x6f,
    0x00, 0x75, 0x62, 0x69, 0x63, 0x61, 0x63, 0x69, 0xc3, 0xb3, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00,
    0x18, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x1c, 0x00,
    0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
    0x21, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x63, 0x6f,
    0x6c, 0x6f, 0x72, 0x00, 0x65, 0x73, 0x74, 0x61, 0x64, 0x6f, 0x00, 0x75, 0x62, 0x69, 0x63, 0x61, 0x63, 0x69,
    0xc3, 0xb3, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x63, 0x61, 0x6d, 0x62,
    0x69, 0x61, 0x72, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x00, 0x63, 0x61, 0x6d, 0x62, 0x69, 0x61, 0x72, 0x45, 0x73,
    0x74, 0x61, 0x64, 0x6f, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0xd0, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00,
    0x30, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00,
    0x00, 0x00, 0x4c, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x0c, 0x01, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00,
    0x06, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x4c, 0x00,
    0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x74, 0x00, 0x00, 0x00,
    0x60, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x88, 0x00,
    0x00, 0x00, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x30, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xac, 0x00,
    0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00,
    0xc0, 0x00, 0x00, 0x00, 0xac, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00,
    0x00, 0x00, 0xd4, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0x01, 0x00, 0x00, 0x00, 0xe8, 0x00, 0x00, 0x00, 0xd4, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0xff, 0xff,
    0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0xe8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0xff, 0xff,
    0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x0c, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x34, 0x01, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x0c, 0x00,
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x48, 0x01, 0x00, 0x00, 0x34, 0x01, 0x00, 0x00,
    0x0d, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x5c, 0x01, 0x00, 0x00, 0x48, 0x01,
    0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x70, 0x01, 0x00, 0x00,
    0x5c, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x9c, 0x01, 0x00, 0x00,
    0xe8, 0x01, 0x00, 0x00, 0x5c, 0x02, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xff, 0xff,
    0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xb0, 0x01, 0x00, 0x00, 0x9c, 0x01, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xc4, 0x01, 0x00, 0x00, 0xb0, 0x01, 0x00, 0x00, 0x03, 0x00,
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xd8, 0x01, 0x00, 0x00, 0xc4, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x0c, 0x00,
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xe8, 0x01, 0x00, 0x00,
    0x0d, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x10, 0x02, 0x00, 0x00, 0xfc, 0x01,
    0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x24, 0x02, 0x00, 0x00,
    0x10, 0x02, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x38, 0x02,
    0x00, 0x00, 0x24, 0x02, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00,
    0x4c, 0x02, 0x00, 0x00, 0x38, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00,
    0x70, 0x02, 0x00, 0x00, 0x5c, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00,
    0x00, 0x00, 0x84, 0x02, 0x00, 0x00, 0x70, 0x02, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0x01, 0x00, 0x00, 0x00, 0x98, 0x02, 0x00, 0x00, 0x84, 0x02, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0xff, 0xff,
    0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xac, 0x02, 0x00, 0x00, 0x98, 0x02, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xc0, 0x02, 0x00, 0x00, 0xac, 0x02, 0x00, 0x00, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00, 0xf0, 0x02, 0x00, 0x00, 0x28, 0x03, 0x00, 0x00, 0x4c, 0x03,
    0x00, 0x00, 0x98, 0x03, 0x00, 0x00, 0xd0, 0x02, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0x01, 0x00, 0x00, 0x00, 0x04, 0x03, 0x00, 0x00, 0xf0, 0x02, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xff, 0xff,
    0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x18, 0x03, 0x00, 0x00, 0x04, 0x03, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd0, 0x02, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xff, 0xff,
    0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x3c, 0x03, 0x00, 0x00, 0x28, 0x03, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd0, 0x02, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0xff, 0xff,
    0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x60, 0x03, 0x00, 0x00, 0x4c, 0x03, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x74, 0x03, 0x00, 0x00, 0x60, 0x03, 0x00, 0x00, 0x0b, 0x00,
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x88, 0x03, 0x00, 0x00, 0x74, 0x03, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd0, 0x02, 0x00, 0x00, 0x0d, 0x00,
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xac, 0x03, 0x00, 0x00, 0x98, 0x03, 0x00, 0x00,
    0x0b, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0xac, 0x03,
    0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};