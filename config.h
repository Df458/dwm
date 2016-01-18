/* See LICENSE file for copyright and license details. */
#include  <X11/XF86keysym.h>

/* appearance */
static const char *fonts[] = {
	"Damnzen:size=8"
};
static const char dmenufont[]       = "Damnzen 8";
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const unsigned int gappx     = 4;
#define NUMCOLORS                     6
static const char colors[NUMCOLORS][MAXCOLORS][8] = {
  // border  foreground background
  { "#111111", "#777777", "#222222" }, // normal
  { "#999999", "#dddddd", "#222222" }, // selected
  { "#81c0c5", "#81c0c5", "#222222" }, // light blue, indicates activity
  { "#f47e7d", "#f47e7d", "#222222" }, // orange,     indicates caution
  { "#fefe55", "#fefe55", "#222222" }, // yellow,     indicates mail
  { "#95f067", "#95f067", "#222222" }, // green
  // add more here
};

/* tagging */
static const char *tags[] = { "_", "_", "_", "_", "_", "_", "_" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",        NULL,      NULL,       1 << 2,       0,        0 },
	{ "Chromium",    NULL,      NULL,       1 << 1,       0,        0 },
	{ "Firefox",     NULL,      NULL,       1 << 1,       0,        0 },
	{ "Singularity", NULL,      NULL,       1 << 3,       0,        0 },
	{ "Sorthelper",  NULL,      NULL,       1 << 4,       0,        0 },
	{ "Pxhelp",      NULL,      NULL,       1 << 4,       0,        0 },
    { "Blender",     NULL,      NULL,       1 << 2,       0,        0 },
	{ "Steam",       NULL,      NULL,       1 << 5,       1,         0 },
	{ "Steam",       NULL,      "Steam",    1 << 5,       0,        0 },
	{ "mpv",         NULL,      NULL,       1 << 1,       0,        1 },
	{ NULL,          "media-term", NULL,    1 << 0,       0,        1 },
	{ NULL,          "mail-term", NULL,     1 << 0,       0,        0 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "  ",      tile },    /* first entry is default */
	{ "  ",      NULL },    /* no layout function means floating behavior */
	{ "  ",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

#define NOMODTAGKEYS(KEY,TAG) \
	{ 0,                     KEY,      toggleview,            {.ui = 1 << TAG} }, \
	{ ControlMask,           KEY,      view,                  {.ui = 1 << TAG} }, \
	{ ShiftMask,             KEY,      tag,                   {.ui = 1 << TAG} }, \
	{ ControlMask|ShiftMask, KEY,      toggletag,             {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
//static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *dmenucmd[]  = { "dmenu_run", "-nb", colors[0][2], "-nf", colors[0][1], "-sb", colors[1][2], "-sf", colors[1][1], NULL };
static const char *termcmd[]      = { "st", NULL };
static const char *filescmd[]     = { "urxvt", "-e", "ranger", NULL };
static const char *slockcmd[]     = { "slock", NULL };
static const char *spookycmd[]    = { "mpv", "-fs", "/home/df458/Videos/SKELETON.mkv", "--no-osc", NULL };
static const char *pausecmd[]     = { "mpc", "toggle", NULL };
static const char *prevcmd[]      = { "mpc", "prev", NULL };
static const char *nextcmd[]      = { "mpc", "next", NULL };
static const char *stopcmd[]      = { "mpc", "stop", NULL };
static const char *voldowncmd[]   = { "volume", "down", NULL };
static const char *volupcmd[]     = { "volume", "up", NULL };
static const char *volmutecmd[]   = { "volume", "toggle", NULL };
static const char *shotfullcmd[]  = { "shot", NULL };
static const char *shotwincmd[]   = { "shot", "window", NULL };
static const char *shotselcmd[]   = { "shot", "select", NULL };
static const char *shutdowncmd[]  = { "systemctl", "poweroff", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ 0,                            XF86XK_AudioPlay,      spawn,          {.v = pausecmd } },
	{ 0,                            XF86XK_AudioPrev,      spawn,          {.v = prevcmd } },
	{ 0,                            XF86XK_AudioNext,      spawn,          {.v = nextcmd } },
	{ 0,                            XF86XK_AudioStop,      spawn,          {.v = stopcmd } },
	{ 0,                            XF86XK_AudioRaiseVolume,      spawn,          {.v = volupcmd } },
	{ 0,                            XF86XK_AudioLowerVolume,      spawn,          {.v = voldowncmd } },
	{ 0,                            XF86XK_AudioMute,      spawn,          {.v = volmutecmd } },
	{ 0,                            XK_Print,              spawn,          {.v = shotfullcmd } },
	{ ControlMask,                  XK_Print,              spawn,          {.v = shotwincmd } },
	{ ControlMask|ShiftMask,        XK_Print,              spawn,          {.v = shotselcmd } },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      spawn,           {.v = shutdowncmd} },
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_grave,  spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_s,      spawn,          {.v = slockcmd } },
	{ MODKEY,                       XK_r,      spawn,          {.v = filescmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	//{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	//{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	NOMODTAGKEYS(                        XK_KP_End,                   0)
	NOMODTAGKEYS(                        XK_KP_Down,                   1)
	NOMODTAGKEYS(                        XK_KP_Page_Down,                   2)
	NOMODTAGKEYS(                        XK_KP_Left,                   3)
    NOMODTAGKEYS(                        XK_KP_Begin,                   4)
	NOMODTAGKEYS(                        XK_KP_Right,                   5)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
    {0, XK_KP_Home, spawn, {.v = spookycmd} },
	{ MODKEY|ShiftMask,                       XK_KP_Home,      view,           {.ui = 1 << 6 }},
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	//{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	//{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	//{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	//{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	//{ ClkTagBar,            0,              Button1,        view,           {0} },
	//{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	//{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	//{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

