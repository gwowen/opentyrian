/*
 * OpenTyrian Classic: A modern cross-platform port of Tyrian
 * Copyright (C) 2007  The OpenTyrian Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include "opentyr.h"
#include "tyrian2.h"

#include "newshape.h"
#include "fonthand.h"
#include "vga256d.h"
#include "picload.h"
#include "starfade.h"
#include "helptext.h"
#include "nortsong.h"
#include "pallib.h"
#include "pcxmast.h"
#include "keyboard.h"
#include "varz.h"
#include "joystick.h"
#include "setup.h"
#include "mainint.h"
#include "sndmast.h"
#include "params.h"

#include <string.h>

void JE_titleScreen( JE_boolean animate )
{
    JE_boolean quit = 0;

    const int menunum = 7;
    JE_byte namego[SA + 2] = {0}; /* [1..SA+2] */
    JE_word waitForDemo;
    JE_byte menu = 0;
    JE_boolean redraw = TRUE,
               fadeIn = FALSE,
               first = TRUE;
    JE_char flash;
    JE_word z;

    JE_word temp; /* JE_byte temp; from varz.h will overflow in for loop */

    /* TODO JE_initPlayerData(); */

    /*PlayCredits;*/

    /* TODO JE_sortHighScores;*/

    /* TODO if(haltGame)
    {
        JE_tyrianHalt(0);
    }*/

    tempScreenSeg = VGAScreen;

    joystickWaitMax = 80;
    joystickWait = 0;

    gameLoaded = FALSE;
    jumpSection = FALSE;

    /* If IsNetworkActive { TODO } else { */

    do
    {
        defaultBrightness = -3;

        /* Animate instead of quickly fading in */
        if (redraw)
        {
            /* TODO if(currentSong != song_title) JE_playSong(song_title); */
            menu = 0;
            redraw = FALSE;
            if (animate)
            {
                if (fadeIn)
                {
                    JE_fadeBlack(10);
                }
                JE_loadPIC(4, FALSE);

                memcpy(VGAScreen2Seg, VGAScreen->pixels, sizeof(VGAScreen2Seg));

                if (moveTyrianLogoUp)
                {
                    temp = 62;
                } else {
                    temp = 4;
                }

                JE_newDrawCShapeNum(PlanetShapes, 146, 11, temp);


                memcpy(colors2, colors, sizeof(colors));
                for (temp = 256-16; temp < 256; temp++)
                {
                    colors[temp].r = 0;
                    colors[temp].g = 0;
                    colors[temp].b = 0;
                }
                colors2[temp].r = 0;

                JE_showVGA();
                JE_fadeColor(10);

                fadeIn = FALSE;

                if (moveTyrianLogoUp)
                {
                    for (temp = 61; temp >= 4; temp -= 2)
                    {
                        int i;

                        setdelay(2);
                        memcpy(VGAScreen->pixels, VGAScreen2Seg, sizeof(VGAScreen2Seg));

                        JE_newDrawCShapeNum(PlanetShapes, 146, 11, temp);

                        JE_showVGA();
                        wait_delay();
                    }
                }
                moveTyrianLogoUp = FALSE;

                /* Draw Menu Text on Screen */
                for (temp = 0; temp < menunum; temp++)
                {
                    tempX = 104+(temp)*13;
                    tempY = JE_fontCenter(menuText[temp],SmallFontShapes);

                    JE_outTextAdjust(tempY-1,tempX-1,menuText[temp],15,-10,SmallFontShapes,FALSE);
                    JE_outTextAdjust(tempY+1,tempX+1,menuText[temp],15,-10,SmallFontShapes,FALSE);
                    JE_outTextAdjust(tempY+1,tempX-1,menuText[temp],15,-10,SmallFontShapes,FALSE);
                    JE_outTextAdjust(tempY-1,tempX+1,menuText[temp],15,-10,SmallFontShapes,FALSE);
                    JE_outTextAdjust(tempY,tempX,menuText[temp],15,-3,SmallFontShapes,FALSE);
                }
                JE_showVGA();

                JE_fadeColors(&colors, &colors2, 0, 255, 20);

                memcpy(VGAScreen2Seg, VGAScreen->pixels, sizeof(VGAScreen2Seg));
            }
        }

        memcpy(VGAScreen->pixels, VGAScreen2Seg, sizeof(VGAScreen2Seg));

        for (temp = 0; temp < menunum; temp++)
        {
            JE_outTextAdjust(JE_fontCenter(menuText[temp], SmallFontShapes),
            104+temp*13,
            menuText[temp], 15, -3+((temp == menu) * 2), SmallFontShapes, FALSE);
        }

        JE_showVGA();

        first = FALSE;

        if (trentWin)
        {
            quit = TRUE;
            goto trentWinsGame;
        }

        waitForDemo = 2000;
        JE_textMenuWait(&waitForDemo, FALSE);

        switch (lastkey_sym)
        {
            case SDLK_UP:
                if (menu == 0)
                {
                    menu = menunum-1;
                } else {
                    menu--;
                }
                JE_playSampleNum(CursorMove);
                break;
            case SDLK_DOWN:
                if (menu == menunum-1)
                {
                    menu = 0;
                } else {
                    menu++;
                }
                JE_playSampleNum(CursorMove);
                break;
            case SDLK_RETURN:
                JE_playSampleNum(Select);
                switch (menu)
                {
                    case 0: /* New game */
                        JE_fadeBlack(10);
                        if (JE_playerSelect())
                        {
                            if (0 /*netQuit*/)
                            {
                                /* JE_tyrianHalt(9); */
                            }

                            if (JE_episodeSelect() && JE_difficultySelect())
                            {
                                gameLoaded = TRUE;
                            } else {
                                redraw = TRUE;
                                fadeIn = TRUE;
                            }

                            initialDifficulty = difficultyLevel;

                            if (onePlayerAction)
                            {
                                score = 0;
                                pItems[11] = 8;
                            } else {
                                if (twoPlayerMode)
                                {
                                    score = 0;
                                    score2 = 0;
                                    pItems[11] = 11;
                                    difficultyLevel++;
                                    inputDevice1 = 1;
                                    inputDevice2 = 2;
                                } else {
                                    if (richMode)
                                    {
                                        score = 1000000;
                                    } else {
                                        switch (episodeNum)
                                        {
                                            case 1:
                                                score = 10000;
                                                break;
                                            case 2:
                                                score = 15000;
                                                break;
                                            case 3:
                                                score = 20000;
                                                break;
                                            case 4:
                                                score = 30000;
                                                break;
                                        }
                                    }
                                }
                            }
                        }
                        fadeIn = TRUE;
                        break;
                    case 1: /* Load game */
                        /* JE_loadScreen(); */
                        if (!gameLoaded)
                        {
                            redraw = TRUE;
                        }
                        fadeIn = TRUE;
                        break;
                    case 2: /* High scores */
                        /* JE_highScoreScreen(); */
                        fadeIn = TRUE;
                        break;
                    case 3: /* Instructions */
                        JE_helpSystem(1);
                        redraw = TRUE;
                        fadeIn = TRUE;
                        break;
                    case 4: /* Ordering info */
                        break;
                    case 5: /* Demo */
                        /* JE_initPlayerData(); */
                        playDemo = TRUE;
                        if (playDemoNum++ > 4)
                        {
                            playDemoNum = 0;
                        }
                        break;
                    case 6: /* Quit */
                        quit = TRUE;
                        break;
                }
                if (menu != 4) /* Tweak added to prevent fadeIn when selecting Ordering Info. */
                {
                    redraw = TRUE;
                }
                break;
            case SDLK_ESCAPE:
                quit = TRUE;
                break;
            default:
                break;
        }
    } while (!(quit || gameLoaded || jumpSection || playDemo || loadDestruct));

trentWinsGame:
    JE_fadeBlack(15);
    if (quit)
    {
        /* JE_tyrianHalt(0); */
    }

    /* } (IsNetworkActive) */
}

void JE_openingAnim( void )
{
    /*JE_clr256();*/

    moveTyrianLogoUp = TRUE;

    if (!isNetworkGame && !stoppedDemo)
    {
        memcpy(colors, black, sizeof(colors));
        memset(black, 63, sizeof(black));
        JE_fadeColors(&colors, &black, 0, 255, 50);

        JE_loadPIC(10, FALSE);
        JE_showVGA();

        JE_fadeColors(&black, &colors, 0, 255, 50);
        memset(black, 0, sizeof(black));

        setdelay(200);
        wait_delayorinput(TRUE,TRUE,TRUE);

        JE_fadeBlack(15);

        JE_loadPIC(12, FALSE);
        JE_showVGA();

        memcpy(colors, palettes[pcxpal[11]], sizeof(colors));
        JE_fadeColor(10);

        setdelay(200);
        wait_delayorinput(TRUE,TRUE,TRUE);

        JE_fadeBlack(10);
    }
}
