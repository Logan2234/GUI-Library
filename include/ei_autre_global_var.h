/* Paramètres par défaut des frames */
static ei_anchor_t default_anchor_frame = ei_anc_center;
static ei_relief_t default_relief_frame = ei_relief_none;
static ei_size_t default_frame_size = {100, 100};

/* Paramètres par défaut des buttons */
static ei_anchor_t default_anchor_button = ei_anc_center;
static ei_relief_t default_relief_button = ei_relief_raised;
static ei_size_t default_button_size = {60, 40};

/* Paramètres par défaut des toplevel */
static ei_size_t default_toplevel_size = {320, 240};
static ei_size_t size_divided = {160, 120};
static ei_size_t *default_toplevel_min_size = &size_divided;
static char *default_toplevel_title = "Toplevel";

/* Variables globales de configuration des close_buttons */
static const ei_color_t close_button_color = {0xd2, 0x00, 0x00, 0xff};
static int close_button_border_width = 2;
static int close_button_corner_radius = 6;
static ei_relief_t close_button_relief = ei_relief_raised;
static char *close_button_text = " ";
static ei_anchor_t close_button_anchor = ei_anc_northwest;
static int close_button_width = 17;
static int close_button_height = 17;
static float close_button_rel_x = 0;
static float close_button_rel_y = 0;
static int close_button_x = 10;
static int close_button_y = -25;