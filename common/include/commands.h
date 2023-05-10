/**
 * @file commands.h
 * @brief Commands definition for the different remotes used in the system using the NEC protocol
 * @author Jose Manuel Bravo Pacheco
 * @author Victor Garcia Garcia
 * @date fecha
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
/*General device*/
#define CHANGE_BUTTON 0

/* Device: Liluco IR remote */
/* The Liluco IR remote and receiver work on NEC protocol */
#define LIL_ON_BUTTON 0x00F7C03F    /*!< Liluco IR remote command for button ON */
#define LIL_OFF_BUTTON 0x00F740BF   /*!< Liluco IR remote command for button OFF */
#define LIL_RED_BUTTON 0x00F720DF   /*!< Liluco IR remote command for button RED */
#define LIL_GREEN_BUTTON 0x00F7A05F /*!< Liluco IR remote command for button GREEN */
#define LIL_BLUE_BUTTON 0x00F7609F  /*!< Liluco IR remote command for button BLUE */
#define LIL_WHITE_BUTTON    0x00F7E01F  /*!< Liluco IR remote command for button WHITE */
#define LIL_YELLOW_BUTTON
#define LIL_CYAN_BUTTON
#define LIL_MAGENTA_BUTTON

#define LIL_NUMBER_OF_BUTTONS 9

/* Device: MY LED Strip Controller */
#define MY_RED_BUTTON 0x00FF1AE5    /*!< My IR remote command for button RED */
#define MY_GREEN_BUTTON 0x00FF9A65  /*!< My IR remote command for button GREEN */
#define MY_BLUE_BUTTON 0x00FFA25D   /*!< My IR remote command for button BLUE */


#endif /* COMMANDS_H_ */
