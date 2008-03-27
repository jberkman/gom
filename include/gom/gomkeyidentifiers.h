/*
The MIT License

Copyright (c) 2008 jacob berkman <jacob@ilovegom.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef GOM_KEY_IDENTIFIERS_H
#define GOM_KEY_IDENTIFIERS_H

#include <glib/gmacros.h>

/* http://www.w3.org/TR/2003/NOTE-DOM-Level-3-Events-20031107/keyset.html#KeySet-Set */

G_BEGIN_DECLS

#define GOM_KEY_ACCEPT               "Accept"             /* The Accept (Commit) key. */
#define GOM_KEY_AGAIN                "Again"              /* The Again key. */
#define GOM_KEY_ALL_CANDIDATES       "AllCandidates"      /* The All Candidates key. */
#define GOM_KEY_ALPHANUMERIC         "Alphanumeric"       /* The Alphanumeric key. */
#define GOM_KEY_ALT                  "Alt"                /* The Alt (Menu) key. */
#define GOM_KEY_ALT_GRAPH            "AltGraph"           /* The Alt-Graph key. */
#define GOM_KEY_APPS                 "Apps"               /* The Application key. */
#define GOM_KEY_ATTN                 "Attn"               /* The ATTN key. */
#define GOM_KEY_BROWSER_BACK         "BrowserBack"        /* The Browser Back key. */
#define GOM_KEY_BROWSER_FAVORITES    "BrowserFavorites"   /* The Browser Favorites key. */
#define GOM_KEY_BROWSER_FORWARD      "BrowserForward"     /* The Browser Forward key. */
#define GOM_KEY_BROWSER_HOME         "BrowserHome"        /* The Browser Home key. */
#define GOM_KEY_BROWSER_REFRESH      "BrowserRefresh"     /* The Browser Refresh key. */
#define GOM_KEY_BROWSER_SEARCH       "BrowserSearch"      /* The Browser Search key. */
#define GOM_KEY_BROWSER_STOP         "BrowserStop"        /* The Browser Stop key. */
#define GOM_KEY_CAPS_LOCK            "CapsLock"           /* The Caps Lock (Capital) key. */
#define GOM_KEY_CLEAR                "Clear"              /* The Clear key. */
#define GOM_KEY_CODE_INPUT           "CodeInput"          /* The Code Input key. */
#define GOM_KEY_COMPOSE              "Compose"            /* The Compose key. */
#define GOM_KEY_CONTROL              "Control"            /* The Control (Ctrl) key. */
#define GOM_KEY_CRSEL                "Crsel"              /* The Crsel key. */
#define GOM_KEY_CONVERT              "Convert"            /* The Convert key. */
#define GOM_KEY_COPY                 "Copy"               /* The Copy key. */
#define GOM_KEY_CUT                  "Cut"                /* The Cut key. */
#define GOM_KEY_DOWN                 "Down"               /* The Down Arrow key. */
#define GOM_KEY_END                  "End"                /* The End key. */
#define GOM_KEY_ENTER                "Enter"              /* The Enter key. Note: This key identifier is also used for the Return (Macintosh numpad) key. */
#define GOM_KEY_ERASE_EOF            "EraseEof"           /* The Erase EOF key. */
#define GOM_KEY_EXECUTE              "Execute"            /* The Execute key. */
#define GOM_KEY_EXSEL                "Exsel"              /* The Exsel key. */
#define GOM_KEY_F1                   "F1"                 /* The F1 key. */
#define GOM_KEY_F2                   "F2"                 /* The F2 key. */
#define GOM_KEY_F3                   "F3"                 /* The F3 key. */
#define GOM_KEY_F4                   "F4"                 /* The F4 key. */
#define GOM_KEY_F5                   "F5"                 /* The F5 key. */
#define GOM_KEY_F6                   "F6"                 /* The F6 key. */
#define GOM_KEY_F7                   "F7"                 /* The F7 key. */
#define GOM_KEY_F8                   "F8"                 /* The F8 key. */
#define GOM_KEY_F9                   "F9"                 /* The F9 key. */
#define GOM_KEY_F10                  "F10"                /* The F10 key. */
#define GOM_KEY_F11                  "F11"                /* The F11 key. */
#define GOM_KEY_F12                  "F12"                /* The F12 key. */
#define GOM_KEY_F13                  "F13"                /* The F13 key. */
#define GOM_KEY_F14                  "F14"                /* The F14 key. */
#define GOM_KEY_F15                  "F15"                /* The F15 key. */
#define GOM_KEY_F16                  "F16"                /* The F16 key. */
#define GOM_KEY_F17                  "F17"                /* The F17 key. */
#define GOM_KEY_F18                  "F18"                /* The F18 key. */
#define GOM_KEY_F19                  "F19"                /* The F19 key. */
#define GOM_KEY_F20                  "F20"                /* The F20 key. */
#define GOM_KEY_F21                  "F21"                /* The F21 key. */
#define GOM_KEY_F22                  "F22"                /* The F22 key. */
#define GOM_KEY_F23                  "F23"                /* The F23 key. */
#define GOM_KEY_F24                  "F24"                /* The F24 key. */
#define GOM_KEY_F25                  "F25"
#define GOM_KEY_F26                  "F26"
#define GOM_KEY_F27                  "F27"
#define GOM_KEY_F28                  "F28"
#define GOM_KEY_F29                  "F29"
#define GOM_KEY_F30                  "F30"
#define GOM_KEY_F31                  "F31"
#define GOM_KEY_F32                  "F32"
#define GOM_KEY_F33                  "F33"
#define GOM_KEY_F34                  "F34"
#define GOM_KEY_F35                  "F35"
#define GOM_KEY_FINAL_MODE           "FinalMode"          /* The Final Mode (Final) key used on some asian keyboards. */
#define GOM_KEY_FIND                 "Find"               /* The Find key. */
#define GOM_KEY_FULL_WIDTH           "FullWidth"          /* The Full-Width Characters key. */
#define GOM_KEY_HALF_WIDTH           "HalfWidth"          /* The Half-Width Characters key. */
#define GOM_KEY_HANGUL_MODE          "HangulMode"         /* The Hangul (Korean characters) Mode key. */
#define GOM_KEY_HANJA_MODE           "HanjaMode"          /* The Hanja (Korean characters) Mode key. */
#define GOM_KEY_HELP                 "Help"               /* The Help key. */
#define GOM_KEY_HIRAGANA             "Hiragana"           /* The Hiragana (Japanese Kana characters) key. */
#define GOM_KEY_HOME                 "Home"               /* The Home key. */
#define GOM_KEY_INSERT               "Insert"             /* The Insert (Ins) key. */
#define GOM_KEY_JAPANESE_HIRAGANA    "JapaneseHiragana"   /* The Japanese-Hiragana key. */
#define GOM_KEY_JAPANESE_KATAKANA    "JapaneseKatakana"   /* The Japanese-Katakana key. */
#define GOM_KEY_JAPANESES_ROMAJI     "JapaneseRomaji"     /* The Japanese-Romaji key. */
#define GOM_KEY_JUNJA_MODE           "JunjaMode"          /* The Junja Mode key. */
#define GOM_KEY_KANA_MODE            "KanaMode"           /* The Kana Mode (Kana Lock) key. */
#define GOM_KEY_KANJI_MODE           "KanjiMode"          /* The Kanji (Japanese name for ideographic characters of Chinese origin) Mode key. */
#define GOM_KEY_KATAKANA             "Katakana"           /* The Katakana (Japanese Kana characters) key. */
#define GOM_KEY_LAUNCH_APPLICATION_1 "LaunchApplication1" /* The Start Application One key. */
#define GOM_KEY_LAUNCH_APPLICATION_2 "LaunchApplication2" /* The Start Application Two key. */
#define GOM_KEY_LAUNCH_MAIL          "LaunchMail"         /* The Start Mail key. */
#define GOM_KEY_LEFT                 "Left"               /* The Left Arrow key. */
#define GOM_KEY_META                 "Meta"               /* The Meta key. */
#define GOM_KEY_MEDIA_NEXT_TRACK     "MediaNextTrack"     /* The Media Next Track key. */
#define GOM_KEY_MEDIA_PLAY_PAUSE     "MediaPlayPause"     /* The Media Play Pause key. */
#define GOM_KEY_MEDIA_PREVIOUS_TRACK "MediaPreviousTrack" /* The Media Previous Track key. */
#define GOM_KEY_MEDIA_STOP           "MediaStop"          /* The Media Stok key. */
#define GOM_KEY_MODE_CHANGE          "ModeChange"         /* The Mode Change key. */
#define GOM_KEY_NONCONVERT           "Nonconvert"         /* The Nonconvert (Don't Convert) key. */
#define GOM_KEY_NUM_LOCK             "NumLock"            /* The Num Lock key. */
#define GOM_KEY_PAGE_DOWN            "PageDown"           /* The Page Down (Next) key. */
#define GOM_KEY_PAGE_UP              "PageUp"             /* The Page Up key. */
#define GOM_KEY_PASTE                "Paste"              /* The Paste key. */
#define GOM_KEY_PAUSE                "Pause"              /* The Pause key. */
#define GOM_KEY_PLAY                 "Play"               /* The Play key. */
#define GOM_KEY_PREVIOUS_CANDIDATE   "PreviousCandidate"  /* The Previous Candidate function key. */
#define GOM_KEY_PRINT_SCREEN         "PrintScreen"        /* The Print Screen (PrintScrn, SnapShot) key. */
#define GOM_KEY_PROCESS              "Process"            /* The Process key. */
#define GOM_KEY_PROPS                "Props"              /* The Props key. */
#define GOM_KEY_RIGHT                "Right"              /* The Right Arrow key. */
#define GOM_KEY_ROMAN_CHARACTERS     "RomanCharacters"    /* The Roman Characters function key. */
#define GOM_KEY_SCROLL               "Scroll"             /* The Scroll Lock key. */
#define GOM_KEY_SELECT               "Select"             /* The Select key. */
#define GOM_KEY_SELECT_MEDIA         "SelectMedia"        /* The Select Media key. */
#define GOM_KEY_SHIFT                "Shift"              /* The Shift key. */
#define GOM_KEY_STOP                 "Stop"               /* The Stop key. */
#define GOM_KEY_UP                   "Up"                 /* The Up Arrow key. */
#define GOM_KEY_UNDO                 "Undo"               /* The Undo key. */
#define GOM_KEY_VOLUME_DOWN          "VolumeDown"         /* The Volume Down key. */
#define GOM_KEY_VOUME_MUTE           "VolumeMute"         /* The Volume Mute key. */
#define GOM_KEY_VOLUME_UP            "VolumeUp"           /* The Volume Up key. */
#define GOM_KEY_WIN                  "Win"                /* The Windows Logo key. */
#define GOM_KEY_ZOOM                 "Zoom"               /* The Zoom key. */
#define GOM_KEY_BACKSPACE            "U+0008"             /* The Backspace (Back) key. */
#define GOM_KEY_TAB                  "U+0009"             /* The Horizontal Tabulation (Tab) key. */
#define GOM_KEY_CANCEL               "U+0018"             /* The Cancel key. */
#define GOM_KEY_ESCAPE               "U+001B"             /* The Escape (Esc) key. */
#define GOM_KEY_SPACE                "U+0020"             /* The Space (Spacebar) key. */
#define GOM_KEY_EXCLAMATION          "U+0021"             /* The Exclamation Mark (Factorial, Bang) key (!). */
#define GOM_KEY_QUOTATION            "U+0022"             /* The Quotation Mark (Quote Double) key ("). */
#define GOM_KEY_OCTOTHORPE           "U+0023"             /* The Number Sign (Pound Sign, Hash, Crosshatch, Octothorpe) key (#). */
#define GOM_KEY_DOLLAR_SIGN          "U+0024"             /* The Dollar Sign (milreis, escudo) key ($). */
#define GOM_KEY_AMPERSAND            "U+0026"             /* The Ampersand key (&). */
#define GOM_KEY_APOSTROPHE           "U+0027"             /* The Apostrophe (Apostrophe-Quote, APL Quote) key ('). */
#define GOM_KEY_LEFT_PAREN           "U+0028"             /* The Left Parenthesis (Opening Parenthesis) key ((). */
#define GOM_KEY_RIGHT_PAREN          "U+0029"             /* The Right Parenthesis (Closing Parenthesis) key ()). */
#define GOM_KEY_ASTERIX              "U+002A"             /* The Asterix (Star) key (*). */
#define GOM_KEY_PLUS                 "U+002B"             /* The Plus Sign (Plus) key (+). */
#define GOM_KEY_COMMA                "U+002C"             /* The Comma (decimal separator) sign key (,). */
#define GOM_KEY_HYPHEN               "U+002D"             /* The Hyphen-minus (hyphen or minus sign) key (-). */
#define GOM_KEY_PERIOD               "U+002E"             /* The Full Stop (period, dot, decimal point) key (.). */
#define GOM_KEY_SLASH                "U+002F"             /* The Solidus (slash, virgule, shilling) key (/). */
#define GOM_KEY_0                    "U+0030"             /* The Digit Zero key (0). */
#define GOM_KEY_1                    "U+0031"             /* The Digit One key (1). */
#define GOM_KEY_2                    "U+0032"             /* The Digit Two key (2). */
#define GOM_KEY_3                    "U+0033"             /* The Digit Three key (3). */
#define GOM_KEY_4                    "U+0034"             /* The Digit Four key (4). */
#define GOM_KEY_5                    "U+0035"             /* The Digit Five key (5). */
#define GOM_KEY_6                    "U+0036"             /* The Digit Six key (6). */
#define GOM_KEY_7                    "U+0037"             /* The Digit Seven key (7). */
#define GOM_KEY_8                    "U+0038"             /* The Digit Eight key (8). */
#define GOM_KEY_9                    "U+0039"             /* The Digit Nine key (9). */
#define GOM_KEY_COLON                "U+003A"             /* The Colon key (:). */
#define GOM_KEY_SEMICOLON            "U+003B"             /* The Semicolon key (;). */
#define GOM_KEY_LESS_THAN            "U+003C"             /* The Less-Than Sign key (<). */
#define GOM_KEY_EQUALS               "U+003D"             /* The Equals Sign key (=). */
#define GOM_KEY_GREATER_THAN         "U+003E"             /* The Greater-Than Sign key (>). */
#define GOM_KEY_QUESTION             "U+003F"             /* The Question Mark key (?). */
#define GOM_KEY_AT                   "U+0040"             /* The Commercial At (@) key. */
#define GOM_KEY_A                    "U+0041"             /* The Latin Capital Letter A key (A). */
#define GOM_KEY_B                    "U+0042"             /* The Latin Capital Letter B key (B). */
#define GOM_KEY_C                    "U+0043"             /* The Latin Capital Letter C key (C). */
#define GOM_KEY_D                    "U+0044"             /* The Latin Capital Letter D key (D). */
#define GOM_KEY_E                    "U+0045"             /* The Latin Capital Letter E key (E). */
#define GOM_KEY_F                    "U+0046"             /* The Latin Capital Letter F key (F). */
#define GOM_KEY_G                    "U+0047"             /* The Latin Capital Letter G key (G). */
#define GOM_KEY_H                    "U+0048"             /* The Latin Capital Letter H key (H). */
#define GOM_KEY_I                    "U+0049"             /* The Latin Capital Letter I key (I). */
#define GOM_KEY_J                    "U+004A"             /* The Latin Capital Letter J key (J). */
#define GOM_KEY_K                    "U+004B"             /* The Latin Capital Letter K key (K). */
#define GOM_KEY_L                    "U+004C"             /* The Latin Capital Letter L key (L). */
#define GOM_KEY_M                    "U+004D"             /* The Latin Capital Letter M key (M). */
#define GOM_KEY_N                    "U+004E"             /* The Latin Capital Letter N key (N). */
#define GOM_KEY_O                    "U+004F"             /* The Latin Capital Letter O key (O). */
#define GOM_KEY_P                    "U+0050"             /* The Latin Capital Letter P key (P). */
#define GOM_KEY_Q                    "U+0051"             /* The Latin Capital Letter Q key (Q). */
#define GOM_KEY_R                    "U+0052"             /* The Latin Capital Letter R key (R). */
#define GOM_KEY_S                    "U+0053"             /* The Latin Capital Letter S key (S). */
#define GOM_KEY_T                    "U+0054"             /* The Latin Capital Letter T key (T). */
#define GOM_KEY_U                    "U+0055"             /* The Latin Capital Letter U key (U). */
#define GOM_KEY_V                    "U+0056"             /* The Latin Capital Letter V key (V). */
#define GOM_KEY_W                    "U+0057"             /* The Latin Capital Letter W key (W). */
#define GOM_KEY_X                    "U+0058"             /* The Latin Capital Letter X key (X). */
#define GOM_KEY_Y                    "U+0059"             /* The Latin Capital Letter Y key (Y). */
#define GOM_KEY_Z                    "U+005A"             /* The Latin Capital Letter Z key (Z). */
#define GOM_KEY_OPEN_BRACKET         "U+005B"             /* The Left Square Bracket (Opening Square Bracket) key ([). */
#define GOM_KEY_BACKSLASH            "U+005C"             /* The Reverse Solidus (Backslash) key (\). */
#define GOM_KEY_CLOSE_BRACKET        "U+005D"             /* The Right Square Bracket (Closing Square Bracket) key (]). */
#define GOM_KEY_CIRCUMFLEX           "U+005E"             /* The Circumflex Accent key (^). */
#define GOM_KEY_UNDERSCORE           "U+005F"             /* The Low Sign (Spacing Underscore, Underscore) key (_). */
#define GOM_KEY_BACK_QUOTE           "U+0060"             /* The Grave Accent (Back Quote) key (`). */
#define GOM_KEY_OPEN_BRACE           "U+007B"             /* The Left Curly Bracket (Opening Curly Bracket, Opening Brace, Brace Left) key ({). */
#define GOM_KEY_PIPE                 "U+007C"             /* The Vertical Line (Vertical Bar, Pipe) key (|). */
#define GOM_KEY_CLOSE_BRACE          "U+007D"             /* The Right Curly Bracket (Closing Curly Bracket, Closing Brace, Brace Right) key (}). */
#define GOM_KEY_DELETE               "U+007F"             /* The Delete (Del) Key. */
#define GOM_KEY_INVERTED_EXCLAMATION "U+00A1"             /* The Inverted Exclamation Mark key (¡). */
#define GOM_KEY_DEAD_GRAVE           "U+0300"             /* The Combining Grave Accent (Greek Varia, Dead Grave) key. */
#define GOM_KEY_DEAD_ACUTE           "U+0301"             /* The Combining Acute Accent (Stress Mark, Greek Oxia, Tonos, Dead Eacute) key. */
#define GOM_KEY_DEAD_CIRCUMFLEX      "U+0302"             /* The Combining Circumflex Accent (Hat, Dead Circumflex) key. */
#define GOM_KEY_DEAD_TILDE           "U+0303"             /* The Combining Tilde (Dead Tilde) key. */
#define GOM_KEY_DEAD_MACRON          "U+0304"             /* The Combining Macron (Long, Dead Macron) key. */
#define GOM_KEY_DEAD_BREVE           "U+0306"             /* The Combining Breve (Short, Dead Breve) key. */
#define GOM_KEY_DEAD_DOT             "U+0307"             /* The Combining Dot Above (Derivative, Dead Above Dot) key. */
#define GOM_KEY_DEAD_UMLAUT          "U+0308"             /* The Combining Diaeresis (Double Dot Abode, Umlaut, Greek Dialytika, Double Derivative, Dead Diaeresis) key. */
#define GOM_KEY_DEAD_RING            "U+030A"             /* The Combining Ring Above (Dead Above Ring) key. */
#define GOM_KEY_DEAD_DOUBLE_ACUTE    "U+030B"             /* The Combining Double Acute Accent (Dead Doubleacute) key. */
#define GOM_KEY_DEAD_CARON           "U+030C"             /* The Combining Caron (Hacek, V Above, Dead Caron) key. */
#define GOM_KEY_DEAD_CEDILLA         "U+0327"             /* The Combining Cedilla (Dead Cedilla) key. */
#define GOM_KEY_DEAD_OGONEK          "U+0328"             /* The Combining Ogonek (Nasal Hook, Dead Ogonek) key. */
#define GOM_KEY_DEAD_IOTA            "U+0345"             /* The Combining Greek Ypogegrammeni (Greek Non-Spacing Iota Below, Iota Subscript, Dead Iota) key. */
#define GOM_KEY_EURO                 "U+20AC"             /* The Euro Currency Sign key (~). */
#define GOM_KEY_DEAD_VOICED          "U+3099"             /* The Combining Katakana-Hiragana Voiced Sound Mark (Dead Voiced Sound) key. */
#define GOM_KEY_DEAD_SEMIVOICED      "U+309A"             /* The Combining Katakana-Hiragana Semi-Voiced Sound Mark (Dead Semivoiced Sound) key. */

G_END_DECLS

#endif /* GOM_KEY_IDENTIFIERS_H */
