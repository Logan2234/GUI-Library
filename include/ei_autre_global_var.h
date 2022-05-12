/* Paramètres par défaut des frames */
static ei_anchor_t default_anchor_frame = ei_anc_center;
static ei_relief_t default_relief_frame = ei_relief_none;
static ei_size_t default_frame_size = {100, 100};

/* Paramètres par défaut des buttons */
static ei_anchor_t default_anchor_button = ei_anc_center;
static ei_relief_t default_relief_button = ei_relief_raised;
static ei_size_t default_button_size = {60, 40};

/* Paramètres par défaut des toplevel */
static int constante_4 = 4;
static ei_axis_set_t default_axis = ei_axis_both;
static ei_bool_t vrai = EI_TRUE;
static ei_size_t default_toplevel_size = {320, 240};

/* Variables globales de configuration des close_buttons */
static const ei_color_t close_button_color = {0xff, 0x00, 0x00, 0xff};
static int close_button_border_width = 2;
static int close_button_corner_radius = 10;
// static ei_relief_t close_button_relief = ei_relief_raised;
static char *close_button_text = " ";
// static ei_font_t close_button_text_font = ;
// static ei_color_t close_button_text_color = ;
// static ei_anchor_t close_button_text_anchor = ;
// static ei_callback_t close_button_callback = ;

static ei_anchor_t close_button_anchor = ei_anc_northwest;
// static int close_button_x;
// static int close_button_y;
static int close_button_width = 18;
static int close_button_height = 18;
static float close_button_rel_x = 0.02;
static float close_button_rel_y = 0.02;
// static float close_button_rel_width;
// static float close_button_rel_height;
