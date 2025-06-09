#define BOLD                "\e[1m"
#define DIM                 "\e[2m"
#define ITALIC              "\e[3m"
#define UNDERLIN            "\e[4m"
#define BLINK               "\e[5m"
#define REVERSED            "\e[7m"
#define STRIKETHRU          "\e[9m"

#define BLACK               "\e[30m"
#define RED                 "\e[31m"
#define GREEN               "\e[32m"
#define YELLOW              "\e[33m"
#define BLUE                "\e[34m"
#define MAGENTA             "\e[35m"
#define CYAN                "\e[36m"
#define WHITE               "\e[37m"
#define GRAY                "\e[90m"

#define FOREGROUND_BLACK    "\e[30m"
#define FOREGROUND_RED      "\e[31m"
#define FOREGROUND_GREEN    "\e[32m"
#define FOREGROUNDYELLOW    "\e[33m"
#define FOREGROUNDBLUE      "\e[34m"
#define FOREGROUNDMAGENTA   "\e[35m"
#define FOREGROUNDCYAN      "\e[36m"
#define FOREGROUNDWHITE     "\e[37m"
#define FOREGROUNDGRAY      "\e[90m"

#define BACKGROUND_BLACK    "\e[40m"
#define BACKGROUND_RED      "\e[41m"
#define BACKGROUND_GREEN    "\e[42m"
#define BACKGROUND_YELLOW   "\e[43m"
#define BACKGROUND_BLUE     "\e[44m"
#define BACKGROUND_WHITE    "\e[47m"
#define BACKGROUND_GRAY     "\e[100m"

#define NOCRAYON            "\e[0m"

#define crayon_to_bold(x)           BOLD        #x NOCRAYON
#define crayon_to_dim(x)            DIM         #x NOCRAYON
#define crayon_to_italic(x)         ITALIC      #x NOCRAYON
#define crayon_to_underline(x)      UNDERLIN    #x NOCRAYON
#define crayon_to_blink(x)          BLINK       #x NOCRAYON
#define crayon_to_reversed(x)       REVERSED    #x NOCRAYON
#define crayon_to_strikethru(x)     STRIKETHRU  #x NOCRAYON

#define crayon_to_black(x)          BLACK       #x NOCRAYON
#define crayon_to_red(x)            RED         #x NOCRAYON
#define crayon_to_green(x)          GREEN       #x NOCRAYON
#define crayon_to_yellow(x)         YELLOW      #x NOCRAYON
#define crayon_to_blue(x)           BLUE        #x NOCRAYON
#define crayon_to_magenta(x)        MAGENTA     #x NOCRAYON
#define crayon_to_cyan(x)           CYAN        #x NOCRAYON
#define crayon_to_white(x)          WHITE       #x NOCRAYON
#define crayon_to_gray(x)           GRAY        #x NOCRAYON

#define crayon_no_effect(x)         NOCRAYON #x NOCRAYON
