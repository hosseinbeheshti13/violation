#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <menu.h>
#include <stdlib.h>
#include <time.h>

#define NORMAL_MENU_TEXT 1
#define CURSOR_MENU_TEXT 2
#define USER_MAX 20

char userr_name[100]; //global char
void create_win(WINDOW *win);
void inbox();
void pass_invild();
void log_invild();
void login_local();
void login_pprivate();
void my_violation();
void registerUser();
void success();
void admin_violate();
void private_history();
void local_history();
void out();
int enter();
int delet_violate();
int edit_violate();
void register_private();
int enter_private();
int menu_information(char *s0, char *s1, char *s2, char *s3, char *s4, char *s5, char *s6, int y, int x, int count);
int make_menu();
struct violate
{
    char violation[100];
    char violation_type[100];
    char violation_explain[100];
    char violation_place[100];
    char user_name[100];
    char time[20];
    char number[20];
}; //struct for violation

struct menu_inform
{
    int item_count;
    char item_string[10][50];
    int background_color;
    int cursor_color;
    int normal_text_color;
    int cursor_text_color;
    int start_y, start_x;
}; //struct for menu

typedef struct
{
    char name[10];
    char password[10];
} User; //struct for login and sign in
User list[USER_MAX];
char *filename = "user.txt";
char *txtname = "private.txt";
User login()
{
    WINDOW *win;
    win = newwin(10, 20, 9, 29);
    create_win(win);
    char name[10];
    char password[10];
    User lg;
    mvprintw(10, 26, " ENTE YOUR USER NAME:");
    scanw("%s", name);
    strcpy(userr_name, name); //copy user to global char
    strcpy(lg.name, name);    //copy in to struct
    mvprintw(12, 26, " ENTE YOUR PASSWORD:");
    scanw("%s", password);
    strcpy(lg.password, password);
    return lg;
} //login
void writeToFile(User u)
{
    FILE *fw = fopen(filename, "a+");
    fprintf(fw, "%s\t", u.name);
    fprintf(fw, "%s\t", u.password);
} //write in to the file
int exist(User u)
{
    int i;
    for (i = 0; i < USER_MAX; i++)
    {
        if (0 == strcmp(list[i].name, u.name) && 0 == strcmp(list[i].password, u.password))
        {
            return 1;
        }
    }
    return -1;
} //compare checking
void registerUser()
{
    WINDOW *win;
    win = newwin(10, 20, 9, 29);
    create_win(win);
    char name[10];
    char password[10];
    User user;
    int i;

    mvprintw(10, 26, " ENTE YOUR USER NAME:");
    scanw("%s", user.name);

    mvprintw(12, 26, " ENTE YOUR PASSWORD:");
    scanw("%s", user.password);

    for (i = 0; i < USER_MAX; i++)
    {
        if (0 == strcmp(list[i].name, user.name))
        {
            pass_invild();
            return;
        }
        if (0 == strcmp(list[i].name, user.name) && 0 == strcmp(list[i].password, user.password)) //compare
        {
            pass_invild();
            return;
        }
    }

    writeToFile(user); //write in to file
} //regist user
int enter()
{
    User test[USER_MAX];
    FILE *fp = fopen(filename, "r");
    int i = 0;
    User u;
    if (NULL == fp)
    {
        FILE *fw = fopen(filename, "a+"); //make file if we dont have file
        return -1;
    }
    for (i = 0; i < USER_MAX; i++)
    {
        char uname[10];
        char upassword[10];
        fscanf(fp, "%s%s", uname, upassword);

        strcpy(list[i].name, uname);
        strcpy(list[i].password, upassword);
    }

    int res = menu_information("LOGIN", "REGIST", "", "", "", "", "", 10, 40, 2); //make menu
    if (res == 0)
    {
        u = login();
        if (1 == exist(u))
        {
            clear();
            login_local();
        }
        else
        {
            clear();
            log_invild();
        }
    }
    else if (res == 1)
    {

        registerUser();
        clear();
        success();
    }
}

int menu_information(char *s0, char *s1, char *s2, char *s3, char *s4, char *s5, char *s6, int y, int x, int count)
{
    struct menu_inform my_menu;

    strcpy(my_menu.item_string[0], s0);
    strcpy(my_menu.item_string[1], s1);
    strcpy(my_menu.item_string[2], s2);
    strcpy(my_menu.item_string[3], s3);
    strcpy(my_menu.item_string[4], s4);
    strcpy(my_menu.item_string[5], s5);
    strcpy(my_menu.item_string[6], s6);

    my_menu.background_color = COLOR_RED;
    my_menu.cursor_color = COLOR_BLACK;
    my_menu.cursor_text_color = COLOR_BLUE;
    my_menu.normal_text_color = COLOR_BLACK;

    my_menu.start_y = y;
    my_menu.start_x = x;
    my_menu.item_count = count;

    int res = make_menu(my_menu);

    return res;
} //get information to make menu

int make_menu(struct menu_inform mdata)
{
    int max_len = -1;
    for (int i = 0; i < mdata.item_count; i++)
        if ((int)strlen(mdata.item_string[i]) > max_len)
            max_len = (int)strlen(mdata.item_string[i]);

    keypad(stdscr, true);
    start_color();
    init_pair(NORMAL_MENU_TEXT, mdata.normal_text_color, mdata.background_color);
    init_pair(CURSOR_MENU_TEXT, mdata.cursor_color, mdata.cursor_text_color);

    attron(COLOR_PAIR(NORMAL_MENU_TEXT));

    for (int i = 0; i < mdata.item_count; i++)
    {
        mvprintw(mdata.start_y + i, mdata.start_x, "%s", mdata.item_string[i]);
        for (int j = 0; j < max_len - strlen(mdata.item_string[i]); j++)
            printw(" ");
    }

    attron(COLOR_PAIR(CURSOR_MENU_TEXT));
    mvprintw(mdata.start_y, mdata.start_x, "%s", mdata.item_string[0]);
    for (int j = 0; j < max_len - strlen(mdata.item_string[0]); j++)
        printw(" ");

    int current_cursor = 0;
    int next_cursor;
    int ch;
    int selection = -1;
    while (selection < 0 && (ch = getch()) != 27)
    {
        switch (ch)
        {
        case KEY_DOWN:
        case 's':
        case 'S':
            next_cursor = (current_cursor + 1) % mdata.item_count;
            break;

        case KEY_UP:
        case 'w':
        case 'W':
            next_cursor = (mdata.item_count + current_cursor - 1) % mdata.item_count;
            break;

        case 10:
            selection = current_cursor;
            break;

        default:
            break;
        }
        if (selection < 0)
        {
            attron(COLOR_PAIR(CURSOR_MENU_TEXT));
            mvprintw(mdata.start_y + next_cursor, mdata.start_x, "%s", mdata.item_string[next_cursor]);
            for (int j = 0; j < max_len - strlen(mdata.item_string[next_cursor]); j++)
                printw(" ");
            attroff(COLOR_PAIR(CURSOR_MENU_TEXT));

            attron(COLOR_PAIR(NORMAL_MENU_TEXT));
            mvprintw(mdata.start_y + current_cursor, mdata.start_x, "%s", mdata.item_string[current_cursor]);
            for (int j = 0; j < max_len - strlen(mdata.item_string[current_cursor]); j++)
                printw(" ");

            current_cursor = next_cursor;
        }
    }
    attroff(COLOR_PAIR(CURSOR_MENU_TEXT));
    attroff(COLOR_PAIR(NORMAL_MENU_TEXT));

    refresh();

    return selection;
}

void my_violation()
{
    struct violate m;
    char b;
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strcpy(m.time, asctime(timeinfo)); //copy current timr in to struct
    initscr();
    WINDOW *win;
    win = newwin(10, 20, 9, 29);
    create_win(win);
    FILE *die = fopen("violation.txt", "a+");
    FILE *life = fopen("instant.txt", "a+");
    mvprintw(4, 4, " type violation:");
    mvgetstr(6, 4, m.violation_type);
    mvprintw(8, 4, "violation:");
    mvgetstr(10, 4, m.violation);
    mvprintw(12, 4, "violation_explain:");
    mvgetstr(14, 4, m.violation_explain);
    mvprintw(16, 4, "violation_place:");
    mvgetstr(18, 4, m.violation_place);
    mvprintw(20, 4, "user name:");
    mvgetstr(22, 4, m.user_name);
    mvprintw(24, 4, "enter violatin count:");
    mvgetstr(24, 27, m.number);
    clear();
    int res = menu_information("instant", "normal", "", "", "", "", "", 10, 40, 2);
    if (res == 0)
    {
        fwrite(&m, sizeof(struct violate), 1, life);
        fclose(life);
        login_local();
    }
    else if (res == 1)
    {
        fwrite(&m, sizeof(struct violate), 1, die);
        fclose(die);
        login_local();
    }
} //enter violation

void inbox()
{
    char b;
    int u = 2;
    struct violate n;
    User userr;
    WINDOW *win;
    win = newwin(26, 80, 0, 0);
    create_win(win);
    FILE *life = fopen("instant.txt", "r");
    FILE *die = fopen("violation.txt", "r");
    int res = menu_information("instant", "normal", "", "", "", "", "", 10, 40, 2);
    if (res == 0)
    {
        clear();
        while (fread(&n, sizeof(struct violate), 1, life))
        {
            if (strcmp(userr_name, n.user_name) == 0)
            {
                mvprintw(u, 4, "user name:");
                mvprintw(u, 15, "%s", n.user_name);
                mvprintw(u, 35, "violation_place:");
                mvprintw(u, 52, "%s", n.violation_place);
                u++;
                mvprintw(u, 4, "type violation:");
                mvprintw(u, 20, "%s", n.violation_type);
                mvprintw(u, 40, "time of violation:");
                mvprintw(u, 60, "%s", n.time);
                u++;
                mvprintw(u, 4, "enter violatin count:");
                mvprintw(u, 29, "%s", n.number);
                u++;
                mvprintw(u, 1, "______________________________________________________________________________");
                u = u + 2;
                refresh();
            }
        }
    }
    else if (res == 1)
    {
        clear();
        while (fread(&n, sizeof(struct violate), 1, die))
        {
            if (strcmp(userr_name, n.user_name) == 0)
            {
                mvprintw(u, 4, "user name:");
                mvprintw(u, 15, "%s", n.user_name);
                mvprintw(u, 35, "violation_place:");
                mvprintw(u, 52, "%s", n.violation_place);
                u++;
                mvprintw(u, 4, "type violation:");
                mvprintw(u, 20, "%s", n.violation_type);
                mvprintw(u, 40, "time of violation:");
                mvprintw(u, 60, "%s", n.time);
                u++;
                mvprintw(u, 4, "enter violatin count:");
                mvprintw(u, 29, "%s", n.number);
                u++;
                mvprintw(u, 1, "______________________________________________________________________________");
                u = u + 2;
                refresh();
            }
        }
    }
    fclose(die);
    fclose(life);
    mvprintw(28, 29, "if you want to exit enter b ");
    b = getch();
    if (b == 'b' || b == 'B')
    {
        clear();
        login_local();
    }
}

void choose()
{
    int res = menu_information("private", "local", "exit", "", "", "", "", 10, 40, 3);
    if (res == 0)
    {
        clear();
        enter_private();
    }
    else if (res == 1)
    {
        clear();
        enter();
    }
    else if (res == 2)
    {
        clear();
        out();
    }
}
void writeToFile_private(User m)
{
    FILE *fw = fopen(txtname, "a+");
    fprintf(fw, "%s\t", m.name);
    fprintf(fw, "%s\t", m.password);
}

void login_local()
{
    int res = menu_information("RECORD VIOLATION", "INBOX", "HISTORY", "EDIT VIOLATION", "DELETE VIOLATE", "EXIT", "", 10, 40, 6);
    if (res == 0)
    {
        my_violation();
    }
    else if (res == 1)
    {
        clear();
        inbox();
    }
    else if (res == 2)
    {
        clear();
        local_history();
    }
    else if (res == 3)
    {
        clear();
        edit_violate();
    }
    else if (res == 4)
    {
        clear();
        delet_violate();
    }
    else if (res == 5)
    {
        clear();
        choose();
    }
}
int main()
{
    initscr();
    choose();
}
void log_invild()
{
    mvprintw(11, 11, ",adPPYba, 8b,dPPYba, 8b,dPPYba,  ,adPPYba,  8b,dPPYba,");
    mvprintw(12, 11, "a8P_____88 88P     Y8 88P     Y8 a8       8a 88P     Y8 ");
    mvprintw(13, 11, "8PP######  88         88         8b       d8 88         ");
    mvprintw(14, 11, " 8b,   ,aa 88         88          8a,   ,a8  88       ");
    mvprintw(15, 11, "  Ybbd8    88         88            YbbdP    88    ");
    mvprintw(20, 25, "your are not register yet");
    refresh();
    sleep(3);
    endwin();
}
void pass_invild()
{

    mvprintw(11, 11, ",adPPYba, 8b,dPPYba, 8b,dPPYba,  ,adPPYba,  8b,dPPYba,");
    mvprintw(12, 11, "a8P_____88 88P     Y8 88P     Y8 a8       8a 88P     Y8 ");
    mvprintw(13, 11, "8PP######  88         88         8b       d8 88         ");
    mvprintw(14, 11, " 8b,   ,aa 88         88          8a,   ,a8  88       ");
    mvprintw(15, 11, "  Ybbd8    88         88            YbbdP    88    ");
    mvprintw(20, 25, "your pass or name invild");
    refresh();
    sleep(3);
    endwin();
}
void create_win(WINDOW *win)
{
    win = newwin(30, 80, 0, 0);
    // start_color();
    //init_pair(1, COLOR_BLACK, COLOR_RED);
    //wbkgd(win, COLOR_PAIR(1));
    refresh();
    box(win, 0, 0);
    wrefresh(win);
    //attron(COLOR_PAIR(1));
}

User login_private()
{
    WINDOW *win;
    win = newwin(10, 20, 9, 29);
    create_win(win);
    char name[10];
    char password[10];
    User vi;
    mvprintw(10, 26, " ENTE YOUR USER NAME:");
    scanw("%s", name);
    strcpy(vi.name, name);
    mvprintw(12, 26, " ENTE YOUR PASSWORD:");
    scanw("%s", password);
    strcpy(vi.password, password);
    return vi;
}

void register_private()
{
    WINDOW *win;
    win = newwin(10, 20, 9, 29);
    create_win(win);
    char name[10];
    char password[10];
    User userr;
    int i;

    mvprintw(10, 26, " ENTE YOUR USER NAME:");
    scanw("%s", userr.name);

    mvprintw(12, 26, " ENTE YOUR PASSWORD:");
    scanw("%s", userr.password);

    for (i = 0; i < USER_MAX; i++)
    {
        if (0 == strcmp(list[i].name, userr.name) && 0 == strcmp(list[i].password, userr.password))
        {
            pass_invild();
            return;
        }
    }

    writeToFile_private(userr);
}
int enter_private()
{
    User test[USER_MAX];
    FILE *fp = fopen(txtname, "r");
    int i = 0;
    User u;
    if (NULL == fp)
    {
        FILE *fw = fopen(txtname, "a+");
        return -1;
    }
    for (i = 0; i < USER_MAX; i++)
    {
        char umname[10];
        char umpassword[10];
        fscanf(fp, "%s%s", umname, umpassword);

        strcpy(list[i].name, umname);
        strcpy(list[i].password, umpassword);
    }

    int res = menu_information("LOGIN", "REGIST", "", "", "", "", "", 10, 40, 2);
    if (res == 0)
    {
        u = login_private();
        if (1 == exist(u))
        {
            clear();
            login_pprivate();
        }
        else
        {
            clear();
            log_invild();
        }
    }
    else if (res == 1)
    {

        register_private();
        clear();
        success();
    }
}
void login_pprivate()
{
    int res = menu_information("VIOLATIONS ", "HISTORy", "blocked user", "EXIT", "", "", "", 10, 40, 4);
    if (res == 0)
    {
        clear();
        admin_violate();
    }
    else if (res == 1)
    {
        clear();
        login_pprivate();
    }
    else if (res == 3)
    {
        clear();
        choose();
    }
}
void success()
{
    mvprintw(11,11,  ",adPPYba, 88       88  ,adPPYba,  ,adPPYba,  ,adPPYba, ,adPPYba, ,adPPYba, ");
    mvprintw(12,11, "I8[    "" 88       88 a8          a8        a8P_____88 I8[    "" I8[    ""  ");
    mvprintw(13,11, "   Y8ba,  88       88 8b         8b         8PP#######   $Y8ba,     Y8ba,  ");
    mvprintw(14,11, "aa    ]8I  8a,   ,a88  8a,   ,aa  8a,   ,aa  8b,   ,aa aa    ]8I aa    ]8I  ");
    mvprintw(15,11, " *YbbdP*    YbbdP8Y8    Ybbd8       Ybbd8      Ybbd8     YbbdP     YbbdP   ");
    mvprintw(20, 25, "sign up succes fuly");
    refresh();
    sleep(3);
    endwin();
}
void out()
{
    mvprintw(9, 11, "                       ##   ,d  ");
    mvprintw(10, 11, "                        ^  88   ");
    mvprintw(11, 11, " ,adPPYba, 8b,     ,d8 88 MM88MMM ");
    mvprintw(12, 11, "a8P_____88  `Y8, ,8P'   88  88  ");
    mvprintw(13, 11, "8PP######    )888(      88  88  ");
    mvprintw(14, 11, " 8b,   ,aa  ,d8   8b,   88  88,   ");
    mvprintw(15, 11, "  Ybbd8   8P       Y8   88   Y888");
    mvprintw(20, 25, "good bye");
    refresh();
    sleep(3);
    endwin();
}

int delet_violate()
{
    WINDOW *win;
    win = newwin(10, 20, 9, 29);
    create_win(win);
    char b;
    int index = -1;
    char number[10];

    FILE *fp = fopen("violation.txt", "r");
    FILE *fi = fopen("instant.txt", "r");
    int res = menu_information("instant", "normal", "", "", "", "", "", 10, 40, 2);
    if (res == 0)
    {
        clear();
        mvprintw(2, 4, "Enter your violatione count: ");
        mvscanw(3, 4, "%s", number);
        fseek(fi, 0, SEEK_END);
        long int pos = ftell(fi);
        int violate_count = (int)(pos / sizeof(struct violate));
        fseek(fi, 0, SEEK_SET);
        struct violate violation[violate_count];
        for (int i = 0; i < violate_count; i++)
        {
            fread(&violation[i], sizeof(struct violate), 1, fi);
            if (strcmp(violation[i].number, number) == 0)
                index = i;
        }
        fclose(fi);
        if (index == -1)
            return 2;
        fi = fopen("instant.txt", "w");
        for (int i = 0; i < violate_count; i++)
        {
            if (i != index)
                fwrite(&violation[i], sizeof(struct violate), 1, fi);
        }
    }
    else if (res == 1)
    {
        clear();
        mvprintw(2, 4, "Enter your violatione count: ");
        mvscanw(3, 4, "%s", number);
        fseek(fp, 0, SEEK_END);
        long int pos = ftell(fp);
        int violate_count = (int)(pos / sizeof(struct violate));
        fseek(fp, 0, SEEK_SET);
        struct violate violation[violate_count];
        for (int i = 0; i < violate_count; i++)
        {
            fread(&violation[i], sizeof(struct violate), 1, fp);
            if (strcmp(violation[i].number, number) == 0)
                index = i;
        }
        fclose(fp);
        if (index == -1)
            return 2;
        fp = fopen("violation.txt", "w");
        for (int i = 0; i < violate_count; i++)
        {
            if (i != index)
                fwrite(&violation[i], sizeof(struct violate), 1, fp);
        }
    }

    fclose(fp);
    fclose(fi);
    mvprintw(28, 29, "if you want to exit enter b ");
    b = getch();
    if (b == 'b' || b == 'B')
    {
        clear();
        login_local();
    }
    return 3;
}
int edit_violate()
{
    WINDOW *win;
    win = newwin(10, 20, 9, 29);
    create_win(win);
    int index = -1;
    char number[10];

    FILE *fp = fopen("violation.txt", "r");
    FILE *fi = fopen("instant.txt", "r");
    int res = menu_information("instant", "normal", "", "", "", "", "", 10, 40, 2);
    if (res == 0)
    {
        clear();
        mvprintw(2, 4, "Enter your violatione count: ");
        mvscanw(3, 4, "%s", number);
        fseek(fi, 0, SEEK_END);
        long int pos = ftell(fi);
        int violate_count = (int)(pos / sizeof(struct violate));
        fseek(fi, 0, SEEK_SET);
        struct violate violation[violate_count];
        for (int i = 0; i < violate_count; i++)
        {
            fread(&violation[i], sizeof(struct violate), 1, fi);
            if (strcmp(violation[i].number, number) == 0)
                index = i;
            fopen("instant.txt", "w");
            my_violation();
        }
        fclose(fi);
        if (index == -1)
            return 2;
        fi = fopen("instant.txt", "w");
        for (int i = 0; i < violate_count; i++)
        {
            if (i != index)
                fwrite(&violation[i], sizeof(struct violate), 1, fi);
        }
    }
    else if (res == 1)
    {
        clear();
        mvprintw(2, 4, "Enter your violatione count: ");
        mvscanw(3, 4, "%s", number);
        fseek(fp, 0, SEEK_END);
        long int pos = ftell(fp);
        int violate_count = (int)(pos / sizeof(struct violate));
        fseek(fp, 0, SEEK_SET);
        struct violate violation[violate_count];
        for (int i = 0; i < violate_count; i++)
        {
            fread(&violation[i], sizeof(struct violate), 1, fp);
            if (strcmp(violation[i].number, number) == 0)
                index = i;
            fopen("violation.txt", "w");
            my_violation();
        }
        fclose(fp);
        if (index == -1)
            return 2;
        fp = fopen("violation.txt", "w");
        for (int i = 0; i < violate_count; i++)
        {
            if (i != index)
                fwrite(&violation[i], sizeof(struct violate), 1, fp);
        }
    }

    fclose(fp);
    fclose(fi);
    return 3;
}

void admin_violate()
{

    int u = 2;
    struct violate n;
    char userr_me[100];
    char userr_count[100];
    WINDOW *win;

    win = newwin(26, 80, 0, 0);
    create_win(win);
    FILE *life = fopen("instant.txt", "r");
    FILE *die = fopen("violation.txt", "r");
    FILE *fp = fopen("reject.txt", "a+");
    FILE *fi = fopen("accept.txt", "a+");
    int res = menu_information("instant", "normal", "", "", "", "", "", 10, 40, 2);
    if (res == 0)
    {
        clear();
        while (fread(&n, sizeof(struct violate), 1, life))
        {

            mvprintw(u, 4, "user name:");
            mvprintw(u, 15, "%s", n.user_name);
            mvprintw(u, 35, "violation_place:");
            mvprintw(u, 52, "%s", n.violation_place);
            u++;
            mvprintw(u, 4, "type violation:");
            mvprintw(u, 20, "%s", n.violation_type);
            mvprintw(u, 40, "time of violation:");
            mvprintw(u, 60, "%s", n.time);
            u++;
            mvprintw(u, 4, "enter violatin count:");
            mvprintw(u, 29, "%s", n.number);
            u++;
            mvprintw(u, 1, "______________________________________________________________________________");
            u = u + 2;
            refresh();
        }
        mvprintw(10, 4, "user name:");
        scanw("%s", userr_me);
        mvprintw(12, 4, "violatione count:");
        scanw("%s", userr_count);
        mvprintw(18, 4, "violatione count:%s", userr_count);
        fseek(life, 0, SEEK_END);
        while (fread(&n, sizeof(struct violate), 1, life))
        {
            if (strcmp(n.user_name, userr_me) == 0 && strcmp(n.number, userr_me) == 0)
            {
                int res = menu_information("accept", "reject", "", "", "", "", "", 10, 40, 2);
                if (res == 0)
                {
                    clear();
                    fwrite(&n, sizeof(struct violate), 1, fi);
                    fclose(fi);
                }
                else if (res == 1)
                {
                    clear();
                    fwrite(&n, sizeof(struct violate), 1, fp);
                    fclose(fp);
                }
            }
        }
    }
    else if (res == 1)
    {
        clear();
        while (fread(&n, sizeof(struct violate), 1, die))
        {
            mvprintw(u, 4, "user name:");
            mvprintw(u, 15, "%s", n.user_name);
            mvprintw(u, 35, "violation_place:");
            mvprintw(u, 52, "%s", n.violation_place);
            u++;
            mvprintw(u, 4, "type violation:");
            mvprintw(u, 20, "%s", n.violation_type);
            mvprintw(u, 40, "time of violation:");
            mvprintw(u, 60, "%s", n.time);
            u++;
            mvprintw(u, 4, "enter violatin count:");
            mvprintw(u, 29, "%s", n.number);
            u++;
            mvprintw(u, 1, "______________________________________________________________________________");
            u = u + 2;
            refresh();
        }
        mvprintw(10, 4, "user name:");
        scanw("%s", userr_me);
        mvprintw(12, 4, "violatione count:");
        scanw("%s", userr_count);
        mvprintw(18, 4, "violatione count:%s", userr_count);
        fseek(die, 0, SEEK_END);
        while (fread(&n, sizeof(struct violate), 1, die))
        {
            if (strcmp(n.user_name, userr_me) == 0 && strcmp(n.number, userr_count) == 0)
            {
                int res = menu_information("accept", "reject", "", "", "", "", "", 10, 40, 2);
                if (res == 0)
                {
                    clear();
                    fwrite(&n, sizeof(struct violate), 1, fi);
                    fclose(fi);
                }
                else if (res == 1)
                {
                    clear();
                    fwrite(&n, sizeof(struct violate), 1, fp);
                    fclose(fp);
                }
            }
        }
    }
}
void local_history()
{

    char b;
    int u = 2;
    struct violate n;
    User userr;
    WINDOW *win;
    win = newwin(26, 80, 0, 0);
    create_win(win);
    FILE *life = fopen("accept.txt", "r");
    FILE *die = fopen("reject.txt", "r");
    int res = menu_information("accept", "reject", "", "", "", "", "", 10, 40, 2);
    if (res == 0)
    {
        clear();
        while (fread(&n, sizeof(struct violate), 1, life))
        {
            if (strcmp(userr_name, n.user_name) == 0)
            {
                mvprintw(u, 4, "user name:");
                mvprintw(u, 15, "%s", n.user_name);
                mvprintw(u, 35, "violation_place:");
                mvprintw(u, 52, "%s", n.violation_place);
                u++;
                mvprintw(u, 4, "type violation:");
                mvprintw(u, 20, "%s", n.violation_type);
                mvprintw(u, 40, "time of violation:");
                mvprintw(u, 60, "%s", n.time);
                u++;
                mvprintw(u, 4, "enter violatin count:");
                mvprintw(u, 29, "%s", n.number);
                u++;
                mvprintw(u, 1, "______________________________________________________________________________");
                u = u + 2;
                refresh();
            }
        }
    }
    else if (res == 1)
    {
        clear();
        while (fread(&n, sizeof(struct violate), 1, die))
        {
            if (strcmp(userr_name, n.user_name) == 0)
            {
                mvprintw(u, 4, "user name:");
                mvprintw(u, 15, "%s", n.user_name);
                mvprintw(u, 35, "violation_place:");
                mvprintw(u, 52, "%s", n.violation_place);
                u++;
                mvprintw(u, 4, "type violation:");
                mvprintw(u, 20, "%s", n.violation_type);
                mvprintw(u, 40, "time of violation:");
                mvprintw(u, 60, "%s", n.time);
                u++;
                mvprintw(u, 4, "enter violatin count:");
                mvprintw(u, 29, "%s", n.number);
                u++;
                mvprintw(u, 1, "______________________________________________________________________________");
                u = u + 2;
                refresh();
            }
        }
    }
    fclose(die);
    fclose(life);
    mvprintw(28, 29, "if you want to exit enter b ");
    b = getch();
    if (b == 'b' || b == 'B')
    {
        clear();
        login_local();
    }
}

void private_history()
{

    char b;
    int u = 2;
    struct violate n;
    User userr;
    WINDOW *win;
    win = newwin(26, 80, 0, 0);
    create_win(win);
    FILE *life = fopen("accept.txt", "r");
    FILE *die = fopen("reject.txt", "r");
    int res = menu_information("accept", "reject", "", "", "", "", "", 10, 40, 2);
    if (res == 0)
    {
        clear();
        while (fread(&n, sizeof(struct violate), 1, life))
        {

            mvprintw(u, 4, "user name:");
            mvprintw(u, 15, "%s", n.user_name);
            mvprintw(u, 35, "violation_place:");
            mvprintw(u, 52, "%s", n.violation_place);
            u++;
            mvprintw(u, 4, "type violation:");
            mvprintw(u, 20, "%s", n.violation_type);
            mvprintw(u, 40, "time of violation:");
            mvprintw(u, 60, "%s", n.time);
            u++;
            mvprintw(u, 4, "enter violatin count:");
            mvprintw(u, 29, "%s", n.number);
            u++;
            mvprintw(u, 1, "______________________________________________________________________________");
            u = u + 2;
            refresh();
        }
    }
    else if (res == 1)
    {
        clear();
        while (fread(&n, sizeof(struct violate), 1, die))
        {
            mvprintw(u, 4, "user name:");
            mvprintw(u, 15, "%s", n.user_name);
            mvprintw(u, 35, "violation_place:");
            mvprintw(u, 52, "%s", n.violation_place);
            u++;
            mvprintw(u, 4, "type violation:");
            mvprintw(u, 20, "%s", n.violation_type);
            mvprintw(u, 40, "time of violation:");
            mvprintw(u, 60, "%s", n.time);
            u++;
            mvprintw(u, 4, "enter violatin count:");
            mvprintw(u, 29, "%s", n.number);
            u++;
            mvprintw(u, 1, "______________________________________________________________________________");
            u = u + 2;
            refresh();
        }
    }
}