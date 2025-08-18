#include <tree_sitter/parser.h>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#define LANGUAGE_VERSION 14
#define STATE_COUNT 133
#define LARGE_STATE_COUNT 2
#define SYMBOL_COUNT 64
#define ALIAS_COUNT 0
#define TOKEN_COUNT 37
#define EXTERNAL_TOKEN_COUNT 0
#define FIELD_COUNT 9
#define MAX_ALIAS_SEQUENCE_LENGTH 8
#define PRODUCTION_ID_COUNT 8

enum {
  anon_sym_mod = 1,
  anon_sym_LBRACE = 2,
  anon_sym_RBRACE = 3,
  anon_sym_COLON_COLON = 4,
  anon_sym_LPAREN = 5,
  anon_sym_RPAREN = 6,
  anon_sym_DASH_GT = 7,
  anon_sym_COMMA = 8,
  anon_sym_COLON = 9,
  anon_sym_use = 10,
  anon_sym_SEMI = 11,
  anon_sym_DOT = 12,
  anon_sym_return = 13,
  anon_sym_PLUS = 14,
  anon_sym_DASH = 15,
  anon_sym_STAR = 16,
  anon_sym_SLASH = 17,
  anon_sym_EQ_EQ = 18,
  anon_sym_BANG_EQ = 19,
  anon_sym_LT = 20,
  anon_sym_GT = 21,
  anon_sym_LT_EQ = 22,
  anon_sym_GT_EQ = 23,
  anon_sym_s32 = 24,
  anon_sym_u32 = 25,
  anon_sym_s64 = 26,
  anon_sym_u64 = 27,
  anon_sym_f32 = 28,
  anon_sym_f64 = 29,
  anon_sym_bool = 30,
  anon_sym_void = 31,
  anon_sym_char = 32,
  anon_sym_str_BQUOTE = 33,
  sym_identifier = 34,
  sym_number = 35,
  sym_comment = 36,
  sym_source_file = 37,
  sym__item = 38,
  sym_module_declaration = 39,
  sym_procedure_declaration = 40,
  sym_parameter_list = 41,
  sym_parameter = 42,
  sym_use_statement = 43,
  sym_use_path = 44,
  sym_simple_path = 45,
  sym_use_group = 46,
  sym_use_item = 47,
  sym_block = 48,
  sym__statement = 49,
  sym_return_statement = 50,
  sym_expression_statement = 51,
  sym_expression = 52,
  sym_binary_expression = 53,
  sym_call_expression = 54,
  sym_argument_list = 55,
  sym_member_expression = 56,
  sym_type = 57,
  aux_sym_source_file_repeat1 = 58,
  aux_sym_parameter_list_repeat1 = 59,
  aux_sym_simple_path_repeat1 = 60,
  aux_sym_use_group_repeat1 = 61,
  aux_sym_block_repeat1 = 62,
  aux_sym_argument_list_repeat1 = 63,
};

static const char * const ts_symbol_names[] = {
  [ts_builtin_sym_end] = "end",
  [anon_sym_mod] = "mod",
  [anon_sym_LBRACE] = "{",
  [anon_sym_RBRACE] = "}",
  [anon_sym_COLON_COLON] = "::",
  [anon_sym_LPAREN] = "(",
  [anon_sym_RPAREN] = ")",
  [anon_sym_DASH_GT] = "->",
  [anon_sym_COMMA] = ",",
  [anon_sym_COLON] = ":",
  [anon_sym_use] = "use",
  [anon_sym_SEMI] = ";",
  [anon_sym_DOT] = ".",
  [anon_sym_return] = "return",
  [anon_sym_PLUS] = "+",
  [anon_sym_DASH] = "-",
  [anon_sym_STAR] = "*",
  [anon_sym_SLASH] = "/",
  [anon_sym_EQ_EQ] = "==",
  [anon_sym_BANG_EQ] = "!=",
  [anon_sym_LT] = "<",
  [anon_sym_GT] = ">",
  [anon_sym_LT_EQ] = "<=",
  [anon_sym_GT_EQ] = ">=",
  [anon_sym_s32] = "s32",
  [anon_sym_u32] = "u32",
  [anon_sym_s64] = "s64",
  [anon_sym_u64] = "u64",
  [anon_sym_f32] = "f32",
  [anon_sym_f64] = "f64",
  [anon_sym_bool] = "bool",
  [anon_sym_void] = "void",
  [anon_sym_char] = "char",
  [anon_sym_str_BQUOTE] = "str`",
  [sym_identifier] = "identifier",
  [sym_number] = "number",
  [sym_comment] = "comment",
  [sym_source_file] = "source_file",
  [sym__item] = "_item",
  [sym_module_declaration] = "module_declaration",
  [sym_procedure_declaration] = "procedure_declaration",
  [sym_parameter_list] = "parameter_list",
  [sym_parameter] = "parameter",
  [sym_use_statement] = "use_statement",
  [sym_use_path] = "use_path",
  [sym_simple_path] = "simple_path",
  [sym_use_group] = "use_group",
  [sym_use_item] = "use_item",
  [sym_block] = "block",
  [sym__statement] = "_statement",
  [sym_return_statement] = "return_statement",
  [sym_expression_statement] = "expression_statement",
  [sym_expression] = "expression",
  [sym_binary_expression] = "binary_expression",
  [sym_call_expression] = "call_expression",
  [sym_argument_list] = "argument_list",
  [sym_member_expression] = "member_expression",
  [sym_type] = "type",
  [aux_sym_source_file_repeat1] = "source_file_repeat1",
  [aux_sym_parameter_list_repeat1] = "parameter_list_repeat1",
  [aux_sym_simple_path_repeat1] = "simple_path_repeat1",
  [aux_sym_use_group_repeat1] = "use_group_repeat1",
  [aux_sym_block_repeat1] = "block_repeat1",
  [aux_sym_argument_list_repeat1] = "argument_list_repeat1",
};

static const TSSymbol ts_symbol_map[] = {
  [ts_builtin_sym_end] = ts_builtin_sym_end,
  [anon_sym_mod] = anon_sym_mod,
  [anon_sym_LBRACE] = anon_sym_LBRACE,
  [anon_sym_RBRACE] = anon_sym_RBRACE,
  [anon_sym_COLON_COLON] = anon_sym_COLON_COLON,
  [anon_sym_LPAREN] = anon_sym_LPAREN,
  [anon_sym_RPAREN] = anon_sym_RPAREN,
  [anon_sym_DASH_GT] = anon_sym_DASH_GT,
  [anon_sym_COMMA] = anon_sym_COMMA,
  [anon_sym_COLON] = anon_sym_COLON,
  [anon_sym_use] = anon_sym_use,
  [anon_sym_SEMI] = anon_sym_SEMI,
  [anon_sym_DOT] = anon_sym_DOT,
  [anon_sym_return] = anon_sym_return,
  [anon_sym_PLUS] = anon_sym_PLUS,
  [anon_sym_DASH] = anon_sym_DASH,
  [anon_sym_STAR] = anon_sym_STAR,
  [anon_sym_SLASH] = anon_sym_SLASH,
  [anon_sym_EQ_EQ] = anon_sym_EQ_EQ,
  [anon_sym_BANG_EQ] = anon_sym_BANG_EQ,
  [anon_sym_LT] = anon_sym_LT,
  [anon_sym_GT] = anon_sym_GT,
  [anon_sym_LT_EQ] = anon_sym_LT_EQ,
  [anon_sym_GT_EQ] = anon_sym_GT_EQ,
  [anon_sym_s32] = anon_sym_s32,
  [anon_sym_u32] = anon_sym_u32,
  [anon_sym_s64] = anon_sym_s64,
  [anon_sym_u64] = anon_sym_u64,
  [anon_sym_f32] = anon_sym_f32,
  [anon_sym_f64] = anon_sym_f64,
  [anon_sym_bool] = anon_sym_bool,
  [anon_sym_void] = anon_sym_void,
  [anon_sym_char] = anon_sym_char,
  [anon_sym_str_BQUOTE] = anon_sym_str_BQUOTE,
  [sym_identifier] = sym_identifier,
  [sym_number] = sym_number,
  [sym_comment] = sym_comment,
  [sym_source_file] = sym_source_file,
  [sym__item] = sym__item,
  [sym_module_declaration] = sym_module_declaration,
  [sym_procedure_declaration] = sym_procedure_declaration,
  [sym_parameter_list] = sym_parameter_list,
  [sym_parameter] = sym_parameter,
  [sym_use_statement] = sym_use_statement,
  [sym_use_path] = sym_use_path,
  [sym_simple_path] = sym_simple_path,
  [sym_use_group] = sym_use_group,
  [sym_use_item] = sym_use_item,
  [sym_block] = sym_block,
  [sym__statement] = sym__statement,
  [sym_return_statement] = sym_return_statement,
  [sym_expression_statement] = sym_expression_statement,
  [sym_expression] = sym_expression,
  [sym_binary_expression] = sym_binary_expression,
  [sym_call_expression] = sym_call_expression,
  [sym_argument_list] = sym_argument_list,
  [sym_member_expression] = sym_member_expression,
  [sym_type] = sym_type,
  [aux_sym_source_file_repeat1] = aux_sym_source_file_repeat1,
  [aux_sym_parameter_list_repeat1] = aux_sym_parameter_list_repeat1,
  [aux_sym_simple_path_repeat1] = aux_sym_simple_path_repeat1,
  [aux_sym_use_group_repeat1] = aux_sym_use_group_repeat1,
  [aux_sym_block_repeat1] = aux_sym_block_repeat1,
  [aux_sym_argument_list_repeat1] = aux_sym_argument_list_repeat1,
};

static const TSSymbolMetadata ts_symbol_metadata[] = {
  [ts_builtin_sym_end] = {
    .visible = false,
    .named = true,
  },
  [anon_sym_mod] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_LBRACE] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_RBRACE] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_COLON_COLON] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_LPAREN] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_RPAREN] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_DASH_GT] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_COMMA] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_COLON] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_use] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_SEMI] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_DOT] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_return] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_PLUS] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_DASH] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_STAR] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_SLASH] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_EQ_EQ] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_BANG_EQ] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_LT] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_GT] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_LT_EQ] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_GT_EQ] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_s32] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_u32] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_s64] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_u64] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_f32] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_f64] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_bool] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_void] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_char] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_str_BQUOTE] = {
    .visible = true,
    .named = false,
  },
  [sym_identifier] = {
    .visible = true,
    .named = true,
  },
  [sym_number] = {
    .visible = true,
    .named = true,
  },
  [sym_comment] = {
    .visible = true,
    .named = true,
  },
  [sym_source_file] = {
    .visible = true,
    .named = true,
  },
  [sym__item] = {
    .visible = false,
    .named = true,
  },
  [sym_module_declaration] = {
    .visible = true,
    .named = true,
  },
  [sym_procedure_declaration] = {
    .visible = true,
    .named = true,
  },
  [sym_parameter_list] = {
    .visible = true,
    .named = true,
  },
  [sym_parameter] = {
    .visible = true,
    .named = true,
  },
  [sym_use_statement] = {
    .visible = true,
    .named = true,
  },
  [sym_use_path] = {
    .visible = true,
    .named = true,
  },
  [sym_simple_path] = {
    .visible = true,
    .named = true,
  },
  [sym_use_group] = {
    .visible = true,
    .named = true,
  },
  [sym_use_item] = {
    .visible = true,
    .named = true,
  },
  [sym_block] = {
    .visible = true,
    .named = true,
  },
  [sym__statement] = {
    .visible = false,
    .named = true,
  },
  [sym_return_statement] = {
    .visible = true,
    .named = true,
  },
  [sym_expression_statement] = {
    .visible = true,
    .named = true,
  },
  [sym_expression] = {
    .visible = true,
    .named = true,
  },
  [sym_binary_expression] = {
    .visible = true,
    .named = true,
  },
  [sym_call_expression] = {
    .visible = true,
    .named = true,
  },
  [sym_argument_list] = {
    .visible = true,
    .named = true,
  },
  [sym_member_expression] = {
    .visible = true,
    .named = true,
  },
  [sym_type] = {
    .visible = true,
    .named = true,
  },
  [aux_sym_source_file_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_parameter_list_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_simple_path_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_use_group_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_block_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_argument_list_repeat1] = {
    .visible = false,
    .named = false,
  },
};

enum {
  field_function = 1,
  field_left = 2,
  field_name = 3,
  field_object = 4,
  field_operator = 5,
  field_property = 6,
  field_return_type = 7,
  field_right = 8,
  field_type = 9,
};

static const char * const ts_field_names[] = {
  [0] = NULL,
  [field_function] = "function",
  [field_left] = "left",
  [field_name] = "name",
  [field_object] = "object",
  [field_operator] = "operator",
  [field_property] = "property",
  [field_return_type] = "return_type",
  [field_right] = "right",
  [field_type] = "type",
};

static const TSFieldMapSlice ts_field_map_slices[PRODUCTION_ID_COUNT] = {
  [1] = {.index = 0, .length = 1},
  [2] = {.index = 1, .length = 2},
  [3] = {.index = 3, .length = 2},
  [4] = {.index = 5, .length = 2},
  [5] = {.index = 7, .length = 1},
  [6] = {.index = 8, .length = 2},
  [7] = {.index = 10, .length = 3},
};

static const TSFieldMapEntry ts_field_map_entries[] = {
  [0] =
    {field_name, 1},
  [1] =
    {field_name, 0},
    {field_type, 2},
  [3] =
    {field_name, 0},
    {field_return_type, 5},
  [5] =
    {field_name, 0},
    {field_return_type, 6},
  [7] =
    {field_function, 0},
  [8] =
    {field_object, 0},
    {field_property, 2},
  [10] =
    {field_left, 0},
    {field_operator, 1},
    {field_right, 2},
};

static const TSSymbol ts_alias_sequences[PRODUCTION_ID_COUNT][MAX_ALIAS_SEQUENCE_LENGTH] = {
  [0] = {0},
};

static const uint16_t ts_non_terminal_alias_map[] = {
  0,
};

static const TSStateId ts_primary_state_ids[STATE_COUNT] = {
  [0] = 0,
  [1] = 1,
  [2] = 2,
  [3] = 3,
  [4] = 4,
  [5] = 5,
  [6] = 6,
  [7] = 7,
  [8] = 8,
  [9] = 9,
  [10] = 10,
  [11] = 11,
  [12] = 12,
  [13] = 13,
  [14] = 14,
  [15] = 15,
  [16] = 16,
  [17] = 13,
  [18] = 12,
  [19] = 14,
  [20] = 11,
  [21] = 21,
  [22] = 22,
  [23] = 21,
  [24] = 24,
  [25] = 22,
  [26] = 24,
  [27] = 27,
  [28] = 28,
  [29] = 29,
  [30] = 30,
  [31] = 31,
  [32] = 29,
  [33] = 30,
  [34] = 34,
  [35] = 34,
  [36] = 36,
  [37] = 37,
  [38] = 38,
  [39] = 39,
  [40] = 40,
  [41] = 41,
  [42] = 41,
  [43] = 43,
  [44] = 37,
  [45] = 38,
  [46] = 46,
  [47] = 36,
  [48] = 48,
  [49] = 31,
  [50] = 46,
  [51] = 48,
  [52] = 52,
  [53] = 53,
  [54] = 54,
  [55] = 55,
  [56] = 53,
  [57] = 57,
  [58] = 58,
  [59] = 59,
  [60] = 60,
  [61] = 54,
  [62] = 57,
  [63] = 60,
  [64] = 59,
  [65] = 52,
  [66] = 58,
  [67] = 67,
  [68] = 68,
  [69] = 69,
  [70] = 70,
  [71] = 68,
  [72] = 72,
  [73] = 55,
  [74] = 69,
  [75] = 75,
  [76] = 76,
  [77] = 67,
  [78] = 72,
  [79] = 79,
  [80] = 80,
  [81] = 81,
  [82] = 82,
  [83] = 83,
  [84] = 84,
  [85] = 85,
  [86] = 86,
  [87] = 85,
  [88] = 88,
  [89] = 80,
  [90] = 82,
  [91] = 79,
  [92] = 81,
  [93] = 84,
  [94] = 94,
  [95] = 95,
  [96] = 96,
  [97] = 97,
  [98] = 98,
  [99] = 99,
  [100] = 100,
  [101] = 99,
  [102] = 102,
  [103] = 103,
  [104] = 104,
  [105] = 104,
  [106] = 106,
  [107] = 107,
  [108] = 108,
  [109] = 109,
  [110] = 107,
  [111] = 111,
  [112] = 108,
  [113] = 113,
  [114] = 114,
  [115] = 114,
  [116] = 116,
  [117] = 117,
  [118] = 118,
  [119] = 119,
  [120] = 120,
  [121] = 117,
  [122] = 122,
  [123] = 123,
  [124] = 124,
  [125] = 125,
  [126] = 122,
  [127] = 125,
  [128] = 116,
  [129] = 129,
  [130] = 118,
  [131] = 123,
  [132] = 124,
};

static bool ts_lex(TSLexer *lexer, TSStateId state) {
  START_LEXER();
  eof = lexer->eof(lexer);
  switch (state) {
    case 0:
      if (eof) ADVANCE(12);
      if (lookahead == '!') ADVANCE(8);
      if (lookahead == '(') ADVANCE(17);
      if (lookahead == ')') ADVANCE(18);
      if (lookahead == '*') ADVANCE(30);
      if (lookahead == '+') ADVANCE(27);
      if (lookahead == ',') ADVANCE(20);
      if (lookahead == '-') ADVANCE(29);
      if (lookahead == '.') ADVANCE(25);
      if (lookahead == '/') ADVANCE(31);
      if (lookahead == ':') ADVANCE(22);
      if (lookahead == ';') ADVANCE(24);
      if (lookahead == '<') ADVANCE(34);
      if (lookahead == '=') ADVANCE(9);
      if (lookahead == '>') ADVANCE(35);
      if (lookahead == 'b') ADVANCE(71);
      if (lookahead == 'c') ADVANCE(64);
      if (lookahead == 'f') ADVANCE(51);
      if (lookahead == 'm') ADVANCE(68);
      if (lookahead == 'r') ADVANCE(62);
      if (lookahead == 's') ADVANCE(52);
      if (lookahead == 'u') ADVANCE(53);
      if (lookahead == 'v') ADVANCE(69);
      if (lookahead == '{') ADVANCE(14);
      if (lookahead == '}') ADVANCE(15);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(0)
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(79);
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 1:
      if (lookahead == '!') ADVANCE(8);
      if (lookahead == '(') ADVANCE(17);
      if (lookahead == ')') ADVANCE(18);
      if (lookahead == '*') ADVANCE(30);
      if (lookahead == '+') ADVANCE(27);
      if (lookahead == ',') ADVANCE(20);
      if (lookahead == '-') ADVANCE(28);
      if (lookahead == '.') ADVANCE(25);
      if (lookahead == '/') ADVANCE(31);
      if (lookahead == ':') ADVANCE(21);
      if (lookahead == ';') ADVANCE(24);
      if (lookahead == '<') ADVANCE(34);
      if (lookahead == '=') ADVANCE(9);
      if (lookahead == '>') ADVANCE(35);
      if (lookahead == 'r') ADVANCE(62);
      if (lookahead == '}') ADVANCE(15);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(1)
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(79);
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 2:
      if (lookahead == ')') ADVANCE(18);
      if (lookahead == '/') ADVANCE(3);
      if (lookahead == '{') ADVANCE(14);
      if (lookahead == '}') ADVANCE(15);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(2)
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(79);
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 3:
      if (lookahead == '*') ADVANCE(5);
      if (lookahead == '/') ADVANCE(81);
      END_STATE();
    case 4:
      if (lookahead == '*') ADVANCE(4);
      if (lookahead == '/') ADVANCE(80);
      if (lookahead != 0) ADVANCE(5);
      END_STATE();
    case 5:
      if (lookahead == '*') ADVANCE(4);
      if (lookahead != 0) ADVANCE(5);
      END_STATE();
    case 6:
      if (lookahead == '-') ADVANCE(10);
      if (lookahead == '/') ADVANCE(3);
      if (lookahead == 'b') ADVANCE(71);
      if (lookahead == 'c') ADVANCE(64);
      if (lookahead == 'f') ADVANCE(51);
      if (lookahead == 's') ADVANCE(52);
      if (lookahead == 'u') ADVANCE(54);
      if (lookahead == 'v') ADVANCE(69);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(6)
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 7:
      if (lookahead == ':') ADVANCE(16);
      END_STATE();
    case 8:
      if (lookahead == '=') ADVANCE(33);
      END_STATE();
    case 9:
      if (lookahead == '=') ADVANCE(32);
      END_STATE();
    case 10:
      if (lookahead == '>') ADVANCE(19);
      END_STATE();
    case 11:
      if (eof) ADVANCE(12);
      if (lookahead == '!') ADVANCE(8);
      if (lookahead == '(') ADVANCE(17);
      if (lookahead == ')') ADVANCE(18);
      if (lookahead == '*') ADVANCE(30);
      if (lookahead == '+') ADVANCE(27);
      if (lookahead == ',') ADVANCE(20);
      if (lookahead == '-') ADVANCE(28);
      if (lookahead == '.') ADVANCE(25);
      if (lookahead == '/') ADVANCE(31);
      if (lookahead == ':') ADVANCE(7);
      if (lookahead == ';') ADVANCE(24);
      if (lookahead == '<') ADVANCE(34);
      if (lookahead == '=') ADVANCE(9);
      if (lookahead == '>') ADVANCE(35);
      if (lookahead == 'm') ADVANCE(68);
      if (lookahead == 'u') ADVANCE(75);
      if (lookahead == '}') ADVANCE(15);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(11)
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 12:
      ACCEPT_TOKEN(ts_builtin_sym_end);
      END_STATE();
    case 13:
      ACCEPT_TOKEN(anon_sym_mod);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 14:
      ACCEPT_TOKEN(anon_sym_LBRACE);
      END_STATE();
    case 15:
      ACCEPT_TOKEN(anon_sym_RBRACE);
      END_STATE();
    case 16:
      ACCEPT_TOKEN(anon_sym_COLON_COLON);
      END_STATE();
    case 17:
      ACCEPT_TOKEN(anon_sym_LPAREN);
      END_STATE();
    case 18:
      ACCEPT_TOKEN(anon_sym_RPAREN);
      END_STATE();
    case 19:
      ACCEPT_TOKEN(anon_sym_DASH_GT);
      END_STATE();
    case 20:
      ACCEPT_TOKEN(anon_sym_COMMA);
      END_STATE();
    case 21:
      ACCEPT_TOKEN(anon_sym_COLON);
      END_STATE();
    case 22:
      ACCEPT_TOKEN(anon_sym_COLON);
      if (lookahead == ':') ADVANCE(16);
      END_STATE();
    case 23:
      ACCEPT_TOKEN(anon_sym_use);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 24:
      ACCEPT_TOKEN(anon_sym_SEMI);
      END_STATE();
    case 25:
      ACCEPT_TOKEN(anon_sym_DOT);
      END_STATE();
    case 26:
      ACCEPT_TOKEN(anon_sym_return);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 27:
      ACCEPT_TOKEN(anon_sym_PLUS);
      END_STATE();
    case 28:
      ACCEPT_TOKEN(anon_sym_DASH);
      END_STATE();
    case 29:
      ACCEPT_TOKEN(anon_sym_DASH);
      if (lookahead == '>') ADVANCE(19);
      END_STATE();
    case 30:
      ACCEPT_TOKEN(anon_sym_STAR);
      END_STATE();
    case 31:
      ACCEPT_TOKEN(anon_sym_SLASH);
      if (lookahead == '*') ADVANCE(5);
      if (lookahead == '/') ADVANCE(81);
      END_STATE();
    case 32:
      ACCEPT_TOKEN(anon_sym_EQ_EQ);
      END_STATE();
    case 33:
      ACCEPT_TOKEN(anon_sym_BANG_EQ);
      END_STATE();
    case 34:
      ACCEPT_TOKEN(anon_sym_LT);
      if (lookahead == '=') ADVANCE(36);
      END_STATE();
    case 35:
      ACCEPT_TOKEN(anon_sym_GT);
      if (lookahead == '=') ADVANCE(37);
      END_STATE();
    case 36:
      ACCEPT_TOKEN(anon_sym_LT_EQ);
      END_STATE();
    case 37:
      ACCEPT_TOKEN(anon_sym_GT_EQ);
      END_STATE();
    case 38:
      ACCEPT_TOKEN(anon_sym_s32);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 39:
      ACCEPT_TOKEN(anon_sym_u32);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 40:
      ACCEPT_TOKEN(anon_sym_s64);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 41:
      ACCEPT_TOKEN(anon_sym_u64);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 42:
      ACCEPT_TOKEN(anon_sym_f32);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 43:
      ACCEPT_TOKEN(anon_sym_f64);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 44:
      ACCEPT_TOKEN(anon_sym_bool);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 45:
      ACCEPT_TOKEN(anon_sym_void);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 46:
      ACCEPT_TOKEN(anon_sym_char);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 47:
      ACCEPT_TOKEN(anon_sym_str_BQUOTE);
      END_STATE();
    case 48:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == '2') ADVANCE(42);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 49:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == '2') ADVANCE(38);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 50:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == '2') ADVANCE(39);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 51:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == '3') ADVANCE(48);
      if (lookahead == '6') ADVANCE(55);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 52:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == '3') ADVANCE(49);
      if (lookahead == '6') ADVANCE(56);
      if (lookahead == 't') ADVANCE(72);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 53:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == '3') ADVANCE(50);
      if (lookahead == '6') ADVANCE(57);
      if (lookahead == 's') ADVANCE(63);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 54:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == '3') ADVANCE(50);
      if (lookahead == '6') ADVANCE(57);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 55:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == '4') ADVANCE(43);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 56:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == '4') ADVANCE(40);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 57:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == '4') ADVANCE(41);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 58:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == '`') ADVANCE(47);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          ('_' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 59:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'a') ADVANCE(73);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('b' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 60:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'd') ADVANCE(13);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 61:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'd') ADVANCE(45);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 62:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'e') ADVANCE(76);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 63:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'e') ADVANCE(23);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 64:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'h') ADVANCE(59);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 65:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'i') ADVANCE(61);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 66:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'l') ADVANCE(44);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 67:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'n') ADVANCE(26);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 68:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'o') ADVANCE(60);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 69:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'o') ADVANCE(65);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 70:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'o') ADVANCE(66);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 71:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'o') ADVANCE(70);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 72:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'r') ADVANCE(58);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 73:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'r') ADVANCE(46);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 74:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'r') ADVANCE(67);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 75:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 's') ADVANCE(63);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 76:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 't') ADVANCE(77);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 77:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'u') ADVANCE(74);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 78:
      ACCEPT_TOKEN(sym_identifier);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(78);
      END_STATE();
    case 79:
      ACCEPT_TOKEN(sym_number);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(79);
      END_STATE();
    case 80:
      ACCEPT_TOKEN(sym_comment);
      END_STATE();
    case 81:
      ACCEPT_TOKEN(sym_comment);
      if (lookahead != 0 &&
          lookahead != '\n') ADVANCE(81);
      END_STATE();
    default:
      return false;
  }
}

static const TSLexMode ts_lex_modes[STATE_COUNT] = {
  [0] = {.lex_state = 0},
  [1] = {.lex_state = 11},
  [2] = {.lex_state = 1},
  [3] = {.lex_state = 1},
  [4] = {.lex_state = 1},
  [5] = {.lex_state = 1},
  [6] = {.lex_state = 1},
  [7] = {.lex_state = 1},
  [8] = {.lex_state = 1},
  [9] = {.lex_state = 11},
  [10] = {.lex_state = 11},
  [11] = {.lex_state = 6},
  [12] = {.lex_state = 1},
  [13] = {.lex_state = 6},
  [14] = {.lex_state = 1},
  [15] = {.lex_state = 6},
  [16] = {.lex_state = 1},
  [17] = {.lex_state = 6},
  [18] = {.lex_state = 1},
  [19] = {.lex_state = 1},
  [20] = {.lex_state = 6},
  [21] = {.lex_state = 11},
  [22] = {.lex_state = 11},
  [23] = {.lex_state = 11},
  [24] = {.lex_state = 11},
  [25] = {.lex_state = 11},
  [26] = {.lex_state = 11},
  [27] = {.lex_state = 11},
  [28] = {.lex_state = 2},
  [29] = {.lex_state = 11},
  [30] = {.lex_state = 11},
  [31] = {.lex_state = 11},
  [32] = {.lex_state = 11},
  [33] = {.lex_state = 11},
  [34] = {.lex_state = 11},
  [35] = {.lex_state = 11},
  [36] = {.lex_state = 11},
  [37] = {.lex_state = 11},
  [38] = {.lex_state = 11},
  [39] = {.lex_state = 2},
  [40] = {.lex_state = 2},
  [41] = {.lex_state = 11},
  [42] = {.lex_state = 11},
  [43] = {.lex_state = 2},
  [44] = {.lex_state = 11},
  [45] = {.lex_state = 11},
  [46] = {.lex_state = 11},
  [47] = {.lex_state = 11},
  [48] = {.lex_state = 11},
  [49] = {.lex_state = 11},
  [50] = {.lex_state = 11},
  [51] = {.lex_state = 11},
  [52] = {.lex_state = 2},
  [53] = {.lex_state = 2},
  [54] = {.lex_state = 2},
  [55] = {.lex_state = 11},
  [56] = {.lex_state = 2},
  [57] = {.lex_state = 2},
  [58] = {.lex_state = 11},
  [59] = {.lex_state = 11},
  [60] = {.lex_state = 11},
  [61] = {.lex_state = 2},
  [62] = {.lex_state = 2},
  [63] = {.lex_state = 11},
  [64] = {.lex_state = 11},
  [65] = {.lex_state = 2},
  [66] = {.lex_state = 11},
  [67] = {.lex_state = 11},
  [68] = {.lex_state = 11},
  [69] = {.lex_state = 11},
  [70] = {.lex_state = 1},
  [71] = {.lex_state = 11},
  [72] = {.lex_state = 2},
  [73] = {.lex_state = 11},
  [74] = {.lex_state = 11},
  [75] = {.lex_state = 1},
  [76] = {.lex_state = 0},
  [77] = {.lex_state = 11},
  [78] = {.lex_state = 2},
  [79] = {.lex_state = 0},
  [80] = {.lex_state = 0},
  [81] = {.lex_state = 0},
  [82] = {.lex_state = 0},
  [83] = {.lex_state = 0},
  [84] = {.lex_state = 2},
  [85] = {.lex_state = 2},
  [86] = {.lex_state = 0},
  [87] = {.lex_state = 2},
  [88] = {.lex_state = 0},
  [89] = {.lex_state = 0},
  [90] = {.lex_state = 0},
  [91] = {.lex_state = 0},
  [92] = {.lex_state = 0},
  [93] = {.lex_state = 2},
  [94] = {.lex_state = 0},
  [95] = {.lex_state = 0},
  [96] = {.lex_state = 0},
  [97] = {.lex_state = 0},
  [98] = {.lex_state = 2},
  [99] = {.lex_state = 2},
  [100] = {.lex_state = 0},
  [101] = {.lex_state = 2},
  [102] = {.lex_state = 2},
  [103] = {.lex_state = 0},
  [104] = {.lex_state = 0},
  [105] = {.lex_state = 0},
  [106] = {.lex_state = 0},
  [107] = {.lex_state = 0},
  [108] = {.lex_state = 2},
  [109] = {.lex_state = 0},
  [110] = {.lex_state = 0},
  [111] = {.lex_state = 0},
  [112] = {.lex_state = 2},
  [113] = {.lex_state = 2},
  [114] = {.lex_state = 2},
  [115] = {.lex_state = 2},
  [116] = {.lex_state = 6},
  [117] = {.lex_state = 0},
  [118] = {.lex_state = 0},
  [119] = {.lex_state = 0},
  [120] = {.lex_state = 0},
  [121] = {.lex_state = 0},
  [122] = {.lex_state = 2},
  [123] = {.lex_state = 0},
  [124] = {.lex_state = 11},
  [125] = {.lex_state = 6},
  [126] = {.lex_state = 2},
  [127] = {.lex_state = 6},
  [128] = {.lex_state = 6},
  [129] = {.lex_state = 1},
  [130] = {.lex_state = 0},
  [131] = {.lex_state = 0},
  [132] = {.lex_state = 11},
};

static const uint16_t ts_parse_table[LARGE_STATE_COUNT][SYMBOL_COUNT] = {
  [0] = {
    [ts_builtin_sym_end] = ACTIONS(1),
    [anon_sym_mod] = ACTIONS(1),
    [anon_sym_LBRACE] = ACTIONS(1),
    [anon_sym_RBRACE] = ACTIONS(1),
    [anon_sym_COLON_COLON] = ACTIONS(1),
    [anon_sym_LPAREN] = ACTIONS(1),
    [anon_sym_RPAREN] = ACTIONS(1),
    [anon_sym_DASH_GT] = ACTIONS(1),
    [anon_sym_COMMA] = ACTIONS(1),
    [anon_sym_COLON] = ACTIONS(1),
    [anon_sym_use] = ACTIONS(1),
    [anon_sym_SEMI] = ACTIONS(1),
    [anon_sym_DOT] = ACTIONS(1),
    [anon_sym_return] = ACTIONS(1),
    [anon_sym_PLUS] = ACTIONS(1),
    [anon_sym_DASH] = ACTIONS(1),
    [anon_sym_STAR] = ACTIONS(1),
    [anon_sym_SLASH] = ACTIONS(1),
    [anon_sym_EQ_EQ] = ACTIONS(1),
    [anon_sym_BANG_EQ] = ACTIONS(1),
    [anon_sym_LT] = ACTIONS(1),
    [anon_sym_GT] = ACTIONS(1),
    [anon_sym_LT_EQ] = ACTIONS(1),
    [anon_sym_GT_EQ] = ACTIONS(1),
    [anon_sym_s32] = ACTIONS(1),
    [anon_sym_u32] = ACTIONS(1),
    [anon_sym_s64] = ACTIONS(1),
    [anon_sym_u64] = ACTIONS(1),
    [anon_sym_f32] = ACTIONS(1),
    [anon_sym_f64] = ACTIONS(1),
    [anon_sym_bool] = ACTIONS(1),
    [anon_sym_void] = ACTIONS(1),
    [anon_sym_char] = ACTIONS(1),
    [anon_sym_str_BQUOTE] = ACTIONS(1),
    [sym_identifier] = ACTIONS(1),
    [sym_number] = ACTIONS(1),
    [sym_comment] = ACTIONS(3),
  },
  [1] = {
    [sym_source_file] = STATE(120),
    [sym__item] = STATE(27),
    [sym_module_declaration] = STATE(27),
    [sym_procedure_declaration] = STATE(27),
    [sym_use_statement] = STATE(27),
    [aux_sym_source_file_repeat1] = STATE(27),
    [ts_builtin_sym_end] = ACTIONS(5),
    [anon_sym_mod] = ACTIONS(7),
    [anon_sym_use] = ACTIONS(9),
    [sym_identifier] = ACTIONS(11),
    [sym_comment] = ACTIONS(3),
  },
};

static const uint16_t ts_small_parse_table[] = {
  [0] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(15), 5,
      anon_sym_return,
      anon_sym_SLASH,
      anon_sym_LT,
      anon_sym_GT,
      sym_identifier,
    ACTIONS(13), 14,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      anon_sym_RPAREN,
      anon_sym_COMMA,
      anon_sym_SEMI,
      anon_sym_DOT,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_EQ_EQ,
      anon_sym_BANG_EQ,
      anon_sym_LT_EQ,
      anon_sym_GT_EQ,
      sym_number,
  [27] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(19), 5,
      anon_sym_return,
      anon_sym_SLASH,
      anon_sym_LT,
      anon_sym_GT,
      sym_identifier,
    ACTIONS(17), 14,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      anon_sym_RPAREN,
      anon_sym_COMMA,
      anon_sym_SEMI,
      anon_sym_DOT,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_EQ_EQ,
      anon_sym_BANG_EQ,
      anon_sym_LT_EQ,
      anon_sym_GT_EQ,
      sym_number,
  [54] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(23), 5,
      anon_sym_return,
      anon_sym_SLASH,
      anon_sym_LT,
      anon_sym_GT,
      sym_identifier,
    ACTIONS(21), 14,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      anon_sym_RPAREN,
      anon_sym_COMMA,
      anon_sym_SEMI,
      anon_sym_DOT,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_EQ_EQ,
      anon_sym_BANG_EQ,
      anon_sym_LT_EQ,
      anon_sym_GT_EQ,
      sym_number,
  [81] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(27), 1,
      anon_sym_DOT,
    ACTIONS(29), 5,
      anon_sym_return,
      anon_sym_SLASH,
      anon_sym_LT,
      anon_sym_GT,
      sym_identifier,
    ACTIONS(25), 13,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      anon_sym_RPAREN,
      anon_sym_COMMA,
      anon_sym_SEMI,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_EQ_EQ,
      anon_sym_BANG_EQ,
      anon_sym_LT_EQ,
      anon_sym_GT_EQ,
      sym_number,
  [110] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(33), 5,
      anon_sym_return,
      anon_sym_SLASH,
      anon_sym_LT,
      anon_sym_GT,
      sym_identifier,
    ACTIONS(31), 14,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      anon_sym_RPAREN,
      anon_sym_COMMA,
      anon_sym_SEMI,
      anon_sym_DOT,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_EQ_EQ,
      anon_sym_BANG_EQ,
      anon_sym_LT_EQ,
      anon_sym_GT_EQ,
      sym_number,
  [137] = 8,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(27), 1,
      anon_sym_DOT,
    ACTIONS(37), 1,
      anon_sym_LPAREN,
    ACTIONS(39), 1,
      anon_sym_SEMI,
    ACTIONS(35), 2,
      anon_sym_RBRACE,
      sym_number,
    ACTIONS(41), 2,
      anon_sym_return,
      sym_identifier,
    ACTIONS(45), 3,
      anon_sym_SLASH,
      anon_sym_LT,
      anon_sym_GT,
    ACTIONS(43), 7,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_EQ_EQ,
      anon_sym_BANG_EQ,
      anon_sym_LT_EQ,
      anon_sym_GT_EQ,
  [172] = 8,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(27), 1,
      anon_sym_DOT,
    ACTIONS(37), 1,
      anon_sym_LPAREN,
    ACTIONS(49), 1,
      anon_sym_SEMI,
    ACTIONS(47), 2,
      anon_sym_RBRACE,
      sym_number,
    ACTIONS(51), 2,
      anon_sym_return,
      sym_identifier,
    ACTIONS(45), 3,
      anon_sym_SLASH,
      anon_sym_LT,
      anon_sym_GT,
    ACTIONS(43), 7,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_EQ_EQ,
      anon_sym_BANG_EQ,
      anon_sym_LT_EQ,
      anon_sym_GT_EQ,
  [207] = 8,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(27), 1,
      anon_sym_DOT,
    ACTIONS(37), 1,
      anon_sym_LPAREN,
    ACTIONS(53), 1,
      anon_sym_RPAREN,
    ACTIONS(55), 1,
      anon_sym_COMMA,
    STATE(83), 1,
      aux_sym_argument_list_repeat1,
    ACTIONS(45), 3,
      anon_sym_SLASH,
      anon_sym_LT,
      anon_sym_GT,
    ACTIONS(43), 7,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_EQ_EQ,
      anon_sym_BANG_EQ,
      anon_sym_LT_EQ,
      anon_sym_GT_EQ,
  [240] = 6,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(27), 1,
      anon_sym_DOT,
    ACTIONS(37), 1,
      anon_sym_LPAREN,
    ACTIONS(57), 2,
      anon_sym_RPAREN,
      anon_sym_COMMA,
    ACTIONS(45), 3,
      anon_sym_SLASH,
      anon_sym_LT,
      anon_sym_GT,
    ACTIONS(43), 7,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_EQ_EQ,
      anon_sym_BANG_EQ,
      anon_sym_LT_EQ,
      anon_sym_GT_EQ,
  [268] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(61), 1,
      anon_sym_str_BQUOTE,
    STATE(107), 1,
      sym_type,
    ACTIONS(59), 10,
      anon_sym_s32,
      anon_sym_u32,
      anon_sym_s64,
      anon_sym_u64,
      anon_sym_f32,
      anon_sym_f64,
      anon_sym_bool,
      anon_sym_void,
      anon_sym_char,
      sym_identifier,
  [290] = 8,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(63), 1,
      anon_sym_RBRACE,
    ACTIONS(65), 1,
      anon_sym_return,
    ACTIONS(67), 1,
      sym_identifier,
    ACTIONS(69), 1,
      sym_number,
    STATE(8), 1,
      sym_expression,
    STATE(2), 3,
      sym_binary_expression,
      sym_call_expression,
      sym_member_expression,
    STATE(14), 4,
      sym__statement,
      sym_return_statement,
      sym_expression_statement,
      aux_sym_block_repeat1,
  [320] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(61), 1,
      anon_sym_str_BQUOTE,
    STATE(105), 1,
      sym_type,
    ACTIONS(59), 10,
      anon_sym_s32,
      anon_sym_u32,
      anon_sym_s64,
      anon_sym_u64,
      anon_sym_f32,
      anon_sym_f64,
      anon_sym_bool,
      anon_sym_void,
      anon_sym_char,
      sym_identifier,
  [342] = 8,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(65), 1,
      anon_sym_return,
    ACTIONS(67), 1,
      sym_identifier,
    ACTIONS(69), 1,
      sym_number,
    ACTIONS(71), 1,
      anon_sym_RBRACE,
    STATE(8), 1,
      sym_expression,
    STATE(2), 3,
      sym_binary_expression,
      sym_call_expression,
      sym_member_expression,
    STATE(16), 4,
      sym__statement,
      sym_return_statement,
      sym_expression_statement,
      aux_sym_block_repeat1,
  [372] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(61), 1,
      anon_sym_str_BQUOTE,
    STATE(111), 1,
      sym_type,
    ACTIONS(59), 10,
      anon_sym_s32,
      anon_sym_u32,
      anon_sym_s64,
      anon_sym_u64,
      anon_sym_f32,
      anon_sym_f64,
      anon_sym_bool,
      anon_sym_void,
      anon_sym_char,
      sym_identifier,
  [394] = 8,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(73), 1,
      anon_sym_RBRACE,
    ACTIONS(75), 1,
      anon_sym_return,
    ACTIONS(78), 1,
      sym_identifier,
    ACTIONS(81), 1,
      sym_number,
    STATE(8), 1,
      sym_expression,
    STATE(2), 3,
      sym_binary_expression,
      sym_call_expression,
      sym_member_expression,
    STATE(16), 4,
      sym__statement,
      sym_return_statement,
      sym_expression_statement,
      aux_sym_block_repeat1,
  [424] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(61), 1,
      anon_sym_str_BQUOTE,
    STATE(104), 1,
      sym_type,
    ACTIONS(59), 10,
      anon_sym_s32,
      anon_sym_u32,
      anon_sym_s64,
      anon_sym_u64,
      anon_sym_f32,
      anon_sym_f64,
      anon_sym_bool,
      anon_sym_void,
      anon_sym_char,
      sym_identifier,
  [446] = 8,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(65), 1,
      anon_sym_return,
    ACTIONS(67), 1,
      sym_identifier,
    ACTIONS(69), 1,
      sym_number,
    ACTIONS(84), 1,
      anon_sym_RBRACE,
    STATE(8), 1,
      sym_expression,
    STATE(2), 3,
      sym_binary_expression,
      sym_call_expression,
      sym_member_expression,
    STATE(19), 4,
      sym__statement,
      sym_return_statement,
      sym_expression_statement,
      aux_sym_block_repeat1,
  [476] = 8,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(65), 1,
      anon_sym_return,
    ACTIONS(67), 1,
      sym_identifier,
    ACTIONS(69), 1,
      sym_number,
    ACTIONS(86), 1,
      anon_sym_RBRACE,
    STATE(8), 1,
      sym_expression,
    STATE(2), 3,
      sym_binary_expression,
      sym_call_expression,
      sym_member_expression,
    STATE(16), 4,
      sym__statement,
      sym_return_statement,
      sym_expression_statement,
      aux_sym_block_repeat1,
  [506] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(61), 1,
      anon_sym_str_BQUOTE,
    STATE(110), 1,
      sym_type,
    ACTIONS(59), 10,
      anon_sym_s32,
      anon_sym_u32,
      anon_sym_s64,
      anon_sym_u64,
      anon_sym_f32,
      anon_sym_f64,
      anon_sym_bool,
      anon_sym_void,
      anon_sym_char,
      sym_identifier,
  [528] = 6,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(88), 1,
      ts_builtin_sym_end,
    ACTIONS(90), 1,
      anon_sym_mod,
    ACTIONS(93), 1,
      anon_sym_use,
    ACTIONS(96), 1,
      sym_identifier,
    STATE(21), 5,
      sym__item,
      sym_module_declaration,
      sym_procedure_declaration,
      sym_use_statement,
      aux_sym_source_file_repeat1,
  [551] = 6,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(99), 1,
      anon_sym_mod,
    ACTIONS(101), 1,
      anon_sym_RBRACE,
    ACTIONS(103), 1,
      anon_sym_use,
    ACTIONS(105), 1,
      sym_identifier,
    STATE(24), 5,
      sym__item,
      sym_module_declaration,
      sym_procedure_declaration,
      sym_use_statement,
      aux_sym_source_file_repeat1,
  [574] = 6,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(88), 1,
      anon_sym_RBRACE,
    ACTIONS(107), 1,
      anon_sym_mod,
    ACTIONS(110), 1,
      anon_sym_use,
    ACTIONS(113), 1,
      sym_identifier,
    STATE(23), 5,
      sym__item,
      sym_module_declaration,
      sym_procedure_declaration,
      sym_use_statement,
      aux_sym_source_file_repeat1,
  [597] = 6,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(99), 1,
      anon_sym_mod,
    ACTIONS(103), 1,
      anon_sym_use,
    ACTIONS(105), 1,
      sym_identifier,
    ACTIONS(116), 1,
      anon_sym_RBRACE,
    STATE(23), 5,
      sym__item,
      sym_module_declaration,
      sym_procedure_declaration,
      sym_use_statement,
      aux_sym_source_file_repeat1,
  [620] = 6,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(99), 1,
      anon_sym_mod,
    ACTIONS(103), 1,
      anon_sym_use,
    ACTIONS(105), 1,
      sym_identifier,
    ACTIONS(118), 1,
      anon_sym_RBRACE,
    STATE(26), 5,
      sym__item,
      sym_module_declaration,
      sym_procedure_declaration,
      sym_use_statement,
      aux_sym_source_file_repeat1,
  [643] = 6,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(99), 1,
      anon_sym_mod,
    ACTIONS(103), 1,
      anon_sym_use,
    ACTIONS(105), 1,
      sym_identifier,
    ACTIONS(120), 1,
      anon_sym_RBRACE,
    STATE(23), 5,
      sym__item,
      sym_module_declaration,
      sym_procedure_declaration,
      sym_use_statement,
      aux_sym_source_file_repeat1,
  [666] = 6,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(7), 1,
      anon_sym_mod,
    ACTIONS(9), 1,
      anon_sym_use,
    ACTIONS(11), 1,
      sym_identifier,
    ACTIONS(122), 1,
      ts_builtin_sym_end,
    STATE(21), 5,
      sym__item,
      sym_module_declaration,
      sym_procedure_declaration,
      sym_use_statement,
      aux_sym_source_file_repeat1,
  [689] = 6,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(124), 1,
      anon_sym_RPAREN,
    STATE(9), 1,
      sym_expression,
    STATE(119), 1,
      sym_argument_list,
    ACTIONS(69), 2,
      sym_identifier,
      sym_number,
    STATE(2), 3,
      sym_binary_expression,
      sym_call_expression,
      sym_member_expression,
  [711] = 5,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(130), 1,
      anon_sym_DOT,
    STATE(29), 1,
      aux_sym_simple_path_repeat1,
    ACTIONS(128), 2,
      anon_sym_RBRACE,
      anon_sym_SEMI,
    ACTIONS(126), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [730] = 5,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(137), 1,
      anon_sym_DOT,
    STATE(35), 1,
      aux_sym_simple_path_repeat1,
    ACTIONS(133), 2,
      ts_builtin_sym_end,
      anon_sym_SEMI,
    ACTIONS(135), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [749] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(141), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
    ACTIONS(139), 4,
      ts_builtin_sym_end,
      anon_sym_RBRACE,
      anon_sym_COMMA,
      anon_sym_SEMI,
  [764] = 5,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(143), 1,
      anon_sym_DOT,
    STATE(32), 1,
      aux_sym_simple_path_repeat1,
    ACTIONS(128), 2,
      ts_builtin_sym_end,
      anon_sym_SEMI,
    ACTIONS(126), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [783] = 5,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(146), 1,
      anon_sym_DOT,
    STATE(34), 1,
      aux_sym_simple_path_repeat1,
    ACTIONS(133), 2,
      anon_sym_RBRACE,
      anon_sym_SEMI,
    ACTIONS(135), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [802] = 5,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(152), 1,
      anon_sym_DOT,
    STATE(29), 1,
      aux_sym_simple_path_repeat1,
    ACTIONS(150), 2,
      anon_sym_RBRACE,
      anon_sym_SEMI,
    ACTIONS(148), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [821] = 5,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(154), 1,
      anon_sym_DOT,
    STATE(32), 1,
      aux_sym_simple_path_repeat1,
    ACTIONS(150), 2,
      ts_builtin_sym_end,
      anon_sym_SEMI,
    ACTIONS(148), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [840] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(158), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
    ACTIONS(156), 4,
      ts_builtin_sym_end,
      anon_sym_RBRACE,
      anon_sym_COMMA,
      anon_sym_SEMI,
  [855] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(162), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
    ACTIONS(160), 4,
      ts_builtin_sym_end,
      anon_sym_RBRACE,
      anon_sym_COMMA,
      anon_sym_SEMI,
  [870] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(166), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
    ACTIONS(164), 4,
      ts_builtin_sym_end,
      anon_sym_RBRACE,
      anon_sym_COMMA,
      anon_sym_SEMI,
  [885] = 4,
    ACTIONS(3), 1,
      sym_comment,
    STATE(5), 1,
      sym_expression,
    ACTIONS(69), 2,
      sym_identifier,
      sym_number,
    STATE(2), 3,
      sym_binary_expression,
      sym_call_expression,
      sym_member_expression,
  [901] = 4,
    ACTIONS(3), 1,
      sym_comment,
    STATE(10), 1,
      sym_expression,
    ACTIONS(69), 2,
      sym_identifier,
      sym_number,
    STATE(2), 3,
      sym_binary_expression,
      sym_call_expression,
      sym_member_expression,
  [917] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(126), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
    ACTIONS(128), 3,
      ts_builtin_sym_end,
      anon_sym_SEMI,
      anon_sym_DOT,
  [931] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(126), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
    ACTIONS(128), 3,
      anon_sym_RBRACE,
      anon_sym_SEMI,
      anon_sym_DOT,
  [945] = 4,
    ACTIONS(3), 1,
      sym_comment,
    STATE(7), 1,
      sym_expression,
    ACTIONS(69), 2,
      sym_identifier,
      sym_number,
    STATE(2), 3,
      sym_binary_expression,
      sym_call_expression,
      sym_member_expression,
  [961] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(160), 2,
      anon_sym_RBRACE,
      anon_sym_SEMI,
    ACTIONS(162), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [974] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(164), 2,
      anon_sym_RBRACE,
      anon_sym_SEMI,
    ACTIONS(166), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [987] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(170), 2,
      anon_sym_RBRACE,
      anon_sym_SEMI,
    ACTIONS(168), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1000] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(156), 2,
      anon_sym_RBRACE,
      anon_sym_SEMI,
    ACTIONS(158), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1013] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(174), 1,
      anon_sym_RBRACE,
    ACTIONS(176), 1,
      anon_sym_SEMI,
    ACTIONS(172), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1028] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(139), 2,
      anon_sym_RBRACE,
      anon_sym_SEMI,
    ACTIONS(141), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1041] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(170), 2,
      ts_builtin_sym_end,
      anon_sym_SEMI,
    ACTIONS(168), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1054] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(174), 1,
      ts_builtin_sym_end,
    ACTIONS(178), 1,
      anon_sym_SEMI,
    ACTIONS(172), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1069] = 5,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(180), 1,
      anon_sym_RBRACE,
    ACTIONS(182), 1,
      sym_identifier,
    STATE(100), 1,
      sym_use_group,
    STATE(106), 1,
      sym_use_item,
  [1085] = 5,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(182), 1,
      sym_identifier,
    ACTIONS(184), 1,
      anon_sym_RBRACE,
    STATE(100), 1,
      sym_use_group,
    STATE(106), 1,
      sym_use_item,
  [1101] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(186), 1,
      sym_identifier,
    STATE(48), 1,
      sym_use_path,
    STATE(46), 2,
      sym_simple_path,
      sym_use_group,
  [1115] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(190), 1,
      anon_sym_RBRACE,
    ACTIONS(188), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1127] = 5,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(182), 1,
      sym_identifier,
    ACTIONS(192), 1,
      anon_sym_RBRACE,
    STATE(100), 1,
      sym_use_group,
    STATE(106), 1,
      sym_use_item,
  [1143] = 5,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(182), 1,
      sym_identifier,
    ACTIONS(194), 1,
      anon_sym_RBRACE,
    STATE(100), 1,
      sym_use_group,
    STATE(106), 1,
      sym_use_item,
  [1159] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(198), 1,
      anon_sym_RBRACE,
    ACTIONS(196), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1171] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(200), 1,
      ts_builtin_sym_end,
    ACTIONS(202), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1183] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(206), 1,
      anon_sym_RBRACE,
    ACTIONS(204), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1195] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(208), 1,
      sym_identifier,
    STATE(51), 1,
      sym_use_path,
    STATE(50), 2,
      sym_simple_path,
      sym_use_group,
  [1209] = 5,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(182), 1,
      sym_identifier,
    ACTIONS(210), 1,
      anon_sym_RBRACE,
    STATE(100), 1,
      sym_use_group,
    STATE(106), 1,
      sym_use_item,
  [1225] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(206), 1,
      ts_builtin_sym_end,
    ACTIONS(204), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1237] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(200), 1,
      anon_sym_RBRACE,
    ACTIONS(202), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1249] = 5,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(182), 1,
      sym_identifier,
    ACTIONS(212), 1,
      anon_sym_RBRACE,
    STATE(100), 1,
      sym_use_group,
    STATE(106), 1,
      sym_use_item,
  [1265] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(198), 1,
      ts_builtin_sym_end,
    ACTIONS(196), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1277] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(216), 1,
      anon_sym_RBRACE,
    ACTIONS(214), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1289] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(218), 1,
      ts_builtin_sym_end,
    ACTIONS(220), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1301] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(224), 1,
      anon_sym_RBRACE,
    ACTIONS(222), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1313] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(226), 2,
      anon_sym_RBRACE,
      sym_number,
    ACTIONS(228), 2,
      anon_sym_return,
      sym_identifier,
  [1325] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(218), 1,
      anon_sym_RBRACE,
    ACTIONS(220), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1337] = 5,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(230), 1,
      anon_sym_RPAREN,
    ACTIONS(232), 1,
      sym_identifier,
    STATE(94), 1,
      sym_parameter,
    STATE(118), 1,
      sym_parameter_list,
  [1353] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(190), 1,
      ts_builtin_sym_end,
    ACTIONS(188), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1365] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(224), 1,
      ts_builtin_sym_end,
    ACTIONS(222), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1377] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(234), 2,
      anon_sym_RBRACE,
      sym_number,
    ACTIONS(236), 2,
      anon_sym_return,
      sym_identifier,
  [1389] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(137), 1,
      anon_sym_DOT,
    STATE(103), 1,
      aux_sym_simple_path_repeat1,
    ACTIONS(238), 2,
      anon_sym_RBRACE,
      anon_sym_COMMA,
  [1403] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(216), 1,
      ts_builtin_sym_end,
    ACTIONS(214), 3,
      anon_sym_mod,
      anon_sym_use,
      sym_identifier,
  [1415] = 5,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(232), 1,
      sym_identifier,
    ACTIONS(240), 1,
      anon_sym_RPAREN,
    STATE(94), 1,
      sym_parameter,
    STATE(130), 1,
      sym_parameter_list,
  [1431] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(242), 1,
      anon_sym_RBRACE,
    ACTIONS(244), 1,
      anon_sym_COMMA,
    STATE(92), 1,
      aux_sym_use_group_repeat1,
  [1444] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(192), 1,
      anon_sym_RBRACE,
    ACTIONS(246), 1,
      anon_sym_COMMA,
    STATE(97), 1,
      aux_sym_use_group_repeat1,
  [1457] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(212), 1,
      anon_sym_RBRACE,
    ACTIONS(248), 1,
      anon_sym_COMMA,
    STATE(97), 1,
      aux_sym_use_group_repeat1,
  [1470] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(212), 1,
      anon_sym_RBRACE,
    ACTIONS(248), 1,
      anon_sym_COMMA,
    STATE(80), 1,
      aux_sym_use_group_repeat1,
  [1483] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(55), 1,
      anon_sym_COMMA,
    ACTIONS(250), 1,
      anon_sym_RPAREN,
    STATE(86), 1,
      aux_sym_argument_list_repeat1,
  [1496] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(182), 1,
      sym_identifier,
    STATE(90), 1,
      sym_use_item,
    STATE(100), 1,
      sym_use_group,
  [1509] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(182), 1,
      sym_identifier,
    STATE(79), 1,
      sym_use_item,
    STATE(100), 1,
      sym_use_group,
  [1522] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(57), 1,
      anon_sym_RPAREN,
    ACTIONS(252), 1,
      anon_sym_COMMA,
    STATE(86), 1,
      aux_sym_argument_list_repeat1,
  [1535] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(182), 1,
      sym_identifier,
    STATE(91), 1,
      sym_use_item,
    STATE(100), 1,
      sym_use_group,
  [1548] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(255), 3,
      anon_sym_LBRACE,
      anon_sym_RPAREN,
      anon_sym_COMMA,
  [1557] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(184), 1,
      anon_sym_RBRACE,
    ACTIONS(257), 1,
      anon_sym_COMMA,
    STATE(97), 1,
      aux_sym_use_group_repeat1,
  [1570] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(180), 1,
      anon_sym_RBRACE,
    ACTIONS(259), 1,
      anon_sym_COMMA,
    STATE(89), 1,
      aux_sym_use_group_repeat1,
  [1583] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(261), 1,
      anon_sym_RBRACE,
    ACTIONS(263), 1,
      anon_sym_COMMA,
    STATE(81), 1,
      aux_sym_use_group_repeat1,
  [1596] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(180), 1,
      anon_sym_RBRACE,
    ACTIONS(259), 1,
      anon_sym_COMMA,
    STATE(97), 1,
      aux_sym_use_group_repeat1,
  [1609] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(182), 1,
      sym_identifier,
    STATE(82), 1,
      sym_use_item,
    STATE(100), 1,
      sym_use_group,
  [1622] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(265), 1,
      anon_sym_RPAREN,
    ACTIONS(267), 1,
      anon_sym_COMMA,
    STATE(95), 1,
      aux_sym_parameter_list_repeat1,
  [1635] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(267), 1,
      anon_sym_COMMA,
    ACTIONS(269), 1,
      anon_sym_RPAREN,
    STATE(96), 1,
      aux_sym_parameter_list_repeat1,
  [1648] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(271), 1,
      anon_sym_RPAREN,
    ACTIONS(273), 1,
      anon_sym_COMMA,
    STATE(96), 1,
      aux_sym_parameter_list_repeat1,
  [1661] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(276), 1,
      anon_sym_RBRACE,
    ACTIONS(278), 1,
      anon_sym_COMMA,
    STATE(97), 1,
      aux_sym_use_group_repeat1,
  [1674] = 4,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(182), 1,
      sym_identifier,
    STATE(100), 1,
      sym_use_group,
    STATE(106), 1,
      sym_use_item,
  [1687] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(281), 1,
      anon_sym_LBRACE,
    ACTIONS(283), 1,
      sym_identifier,
  [1697] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(238), 2,
      anon_sym_RBRACE,
      anon_sym_COMMA,
  [1705] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(285), 1,
      anon_sym_LBRACE,
    ACTIONS(287), 1,
      sym_identifier,
  [1715] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(232), 1,
      sym_identifier,
    STATE(109), 1,
      sym_parameter,
  [1725] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(154), 1,
      anon_sym_DOT,
    STATE(32), 1,
      aux_sym_simple_path_repeat1,
  [1735] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(289), 1,
      anon_sym_LBRACE,
    STATE(66), 1,
      sym_block,
  [1745] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(291), 1,
      anon_sym_LBRACE,
    STATE(58), 1,
      sym_block,
  [1755] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(276), 2,
      anon_sym_RBRACE,
      anon_sym_COMMA,
  [1763] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(289), 1,
      anon_sym_LBRACE,
    STATE(59), 1,
      sym_block,
  [1773] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(283), 1,
      sym_identifier,
    ACTIONS(293), 1,
      anon_sym_LBRACE,
  [1783] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(271), 2,
      anon_sym_RPAREN,
      anon_sym_COMMA,
  [1791] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(291), 1,
      anon_sym_LBRACE,
    STATE(64), 1,
      sym_block,
  [1801] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(295), 2,
      anon_sym_RPAREN,
      anon_sym_COMMA,
  [1809] = 3,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(287), 1,
      sym_identifier,
    ACTIONS(297), 1,
      anon_sym_LBRACE,
  [1819] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(299), 1,
      sym_identifier,
  [1826] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(287), 1,
      sym_identifier,
  [1833] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(283), 1,
      sym_identifier,
  [1840] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(301), 1,
      anon_sym_DASH_GT,
  [1847] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(303), 1,
      anon_sym_LBRACE,
  [1854] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(305), 1,
      anon_sym_RPAREN,
  [1861] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(307), 1,
      anon_sym_RPAREN,
  [1868] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(309), 1,
      ts_builtin_sym_end,
  [1875] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(311), 1,
      anon_sym_LBRACE,
  [1882] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(313), 1,
      sym_identifier,
  [1889] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(315), 1,
      anon_sym_LPAREN,
  [1896] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(317), 1,
      anon_sym_COLON_COLON,
  [1903] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(319), 1,
      anon_sym_DASH_GT,
  [1910] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(321), 1,
      sym_identifier,
  [1917] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(323), 1,
      anon_sym_DASH_GT,
  [1924] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(325), 1,
      anon_sym_DASH_GT,
  [1931] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(327), 1,
      anon_sym_COLON,
  [1938] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(329), 1,
      anon_sym_RPAREN,
  [1945] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(331), 1,
      anon_sym_LPAREN,
  [1952] = 2,
    ACTIONS(3), 1,
      sym_comment,
    ACTIONS(333), 1,
      anon_sym_COLON_COLON,
};

static const uint32_t ts_small_parse_table_map[] = {
  [SMALL_STATE(2)] = 0,
  [SMALL_STATE(3)] = 27,
  [SMALL_STATE(4)] = 54,
  [SMALL_STATE(5)] = 81,
  [SMALL_STATE(6)] = 110,
  [SMALL_STATE(7)] = 137,
  [SMALL_STATE(8)] = 172,
  [SMALL_STATE(9)] = 207,
  [SMALL_STATE(10)] = 240,
  [SMALL_STATE(11)] = 268,
  [SMALL_STATE(12)] = 290,
  [SMALL_STATE(13)] = 320,
  [SMALL_STATE(14)] = 342,
  [SMALL_STATE(15)] = 372,
  [SMALL_STATE(16)] = 394,
  [SMALL_STATE(17)] = 424,
  [SMALL_STATE(18)] = 446,
  [SMALL_STATE(19)] = 476,
  [SMALL_STATE(20)] = 506,
  [SMALL_STATE(21)] = 528,
  [SMALL_STATE(22)] = 551,
  [SMALL_STATE(23)] = 574,
  [SMALL_STATE(24)] = 597,
  [SMALL_STATE(25)] = 620,
  [SMALL_STATE(26)] = 643,
  [SMALL_STATE(27)] = 666,
  [SMALL_STATE(28)] = 689,
  [SMALL_STATE(29)] = 711,
  [SMALL_STATE(30)] = 730,
  [SMALL_STATE(31)] = 749,
  [SMALL_STATE(32)] = 764,
  [SMALL_STATE(33)] = 783,
  [SMALL_STATE(34)] = 802,
  [SMALL_STATE(35)] = 821,
  [SMALL_STATE(36)] = 840,
  [SMALL_STATE(37)] = 855,
  [SMALL_STATE(38)] = 870,
  [SMALL_STATE(39)] = 885,
  [SMALL_STATE(40)] = 901,
  [SMALL_STATE(41)] = 917,
  [SMALL_STATE(42)] = 931,
  [SMALL_STATE(43)] = 945,
  [SMALL_STATE(44)] = 961,
  [SMALL_STATE(45)] = 974,
  [SMALL_STATE(46)] = 987,
  [SMALL_STATE(47)] = 1000,
  [SMALL_STATE(48)] = 1013,
  [SMALL_STATE(49)] = 1028,
  [SMALL_STATE(50)] = 1041,
  [SMALL_STATE(51)] = 1054,
  [SMALL_STATE(52)] = 1069,
  [SMALL_STATE(53)] = 1085,
  [SMALL_STATE(54)] = 1101,
  [SMALL_STATE(55)] = 1115,
  [SMALL_STATE(56)] = 1127,
  [SMALL_STATE(57)] = 1143,
  [SMALL_STATE(58)] = 1159,
  [SMALL_STATE(59)] = 1171,
  [SMALL_STATE(60)] = 1183,
  [SMALL_STATE(61)] = 1195,
  [SMALL_STATE(62)] = 1209,
  [SMALL_STATE(63)] = 1225,
  [SMALL_STATE(64)] = 1237,
  [SMALL_STATE(65)] = 1249,
  [SMALL_STATE(66)] = 1265,
  [SMALL_STATE(67)] = 1277,
  [SMALL_STATE(68)] = 1289,
  [SMALL_STATE(69)] = 1301,
  [SMALL_STATE(70)] = 1313,
  [SMALL_STATE(71)] = 1325,
  [SMALL_STATE(72)] = 1337,
  [SMALL_STATE(73)] = 1353,
  [SMALL_STATE(74)] = 1365,
  [SMALL_STATE(75)] = 1377,
  [SMALL_STATE(76)] = 1389,
  [SMALL_STATE(77)] = 1403,
  [SMALL_STATE(78)] = 1415,
  [SMALL_STATE(79)] = 1431,
  [SMALL_STATE(80)] = 1444,
  [SMALL_STATE(81)] = 1457,
  [SMALL_STATE(82)] = 1470,
  [SMALL_STATE(83)] = 1483,
  [SMALL_STATE(84)] = 1496,
  [SMALL_STATE(85)] = 1509,
  [SMALL_STATE(86)] = 1522,
  [SMALL_STATE(87)] = 1535,
  [SMALL_STATE(88)] = 1548,
  [SMALL_STATE(89)] = 1557,
  [SMALL_STATE(90)] = 1570,
  [SMALL_STATE(91)] = 1583,
  [SMALL_STATE(92)] = 1596,
  [SMALL_STATE(93)] = 1609,
  [SMALL_STATE(94)] = 1622,
  [SMALL_STATE(95)] = 1635,
  [SMALL_STATE(96)] = 1648,
  [SMALL_STATE(97)] = 1661,
  [SMALL_STATE(98)] = 1674,
  [SMALL_STATE(99)] = 1687,
  [SMALL_STATE(100)] = 1697,
  [SMALL_STATE(101)] = 1705,
  [SMALL_STATE(102)] = 1715,
  [SMALL_STATE(103)] = 1725,
  [SMALL_STATE(104)] = 1735,
  [SMALL_STATE(105)] = 1745,
  [SMALL_STATE(106)] = 1755,
  [SMALL_STATE(107)] = 1763,
  [SMALL_STATE(108)] = 1773,
  [SMALL_STATE(109)] = 1783,
  [SMALL_STATE(110)] = 1791,
  [SMALL_STATE(111)] = 1801,
  [SMALL_STATE(112)] = 1809,
  [SMALL_STATE(113)] = 1819,
  [SMALL_STATE(114)] = 1826,
  [SMALL_STATE(115)] = 1833,
  [SMALL_STATE(116)] = 1840,
  [SMALL_STATE(117)] = 1847,
  [SMALL_STATE(118)] = 1854,
  [SMALL_STATE(119)] = 1861,
  [SMALL_STATE(120)] = 1868,
  [SMALL_STATE(121)] = 1875,
  [SMALL_STATE(122)] = 1882,
  [SMALL_STATE(123)] = 1889,
  [SMALL_STATE(124)] = 1896,
  [SMALL_STATE(125)] = 1903,
  [SMALL_STATE(126)] = 1910,
  [SMALL_STATE(127)] = 1917,
  [SMALL_STATE(128)] = 1924,
  [SMALL_STATE(129)] = 1931,
  [SMALL_STATE(130)] = 1938,
  [SMALL_STATE(131)] = 1945,
  [SMALL_STATE(132)] = 1952,
};

static const TSParseActionEntry ts_parse_actions[] = {
  [0] = {.entry = {.count = 0, .reusable = false}},
  [1] = {.entry = {.count = 1, .reusable = false}}, RECOVER(),
  [3] = {.entry = {.count = 1, .reusable = true}}, SHIFT_EXTRA(),
  [5] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 0),
  [7] = {.entry = {.count = 1, .reusable = false}}, SHIFT(122),
  [9] = {.entry = {.count = 1, .reusable = false}}, SHIFT(61),
  [11] = {.entry = {.count = 1, .reusable = false}}, SHIFT(124),
  [13] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_expression, 1),
  [15] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_expression, 1),
  [17] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_call_expression, 3, .production_id = 5),
  [19] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_call_expression, 3, .production_id = 5),
  [21] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_member_expression, 3, .production_id = 6),
  [23] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_member_expression, 3, .production_id = 6),
  [25] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_binary_expression, 3, .production_id = 7),
  [27] = {.entry = {.count = 1, .reusable = true}}, SHIFT(113),
  [29] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_binary_expression, 3, .production_id = 7),
  [31] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_call_expression, 4, .production_id = 5),
  [33] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_call_expression, 4, .production_id = 5),
  [35] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_return_statement, 2),
  [37] = {.entry = {.count = 1, .reusable = true}}, SHIFT(28),
  [39] = {.entry = {.count = 1, .reusable = true}}, SHIFT(75),
  [41] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_return_statement, 2),
  [43] = {.entry = {.count = 1, .reusable = true}}, SHIFT(39),
  [45] = {.entry = {.count = 1, .reusable = false}}, SHIFT(39),
  [47] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_expression_statement, 1),
  [49] = {.entry = {.count = 1, .reusable = true}}, SHIFT(70),
  [51] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_expression_statement, 1),
  [53] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_argument_list, 1),
  [55] = {.entry = {.count = 1, .reusable = true}}, SHIFT(40),
  [57] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_argument_list_repeat1, 2),
  [59] = {.entry = {.count = 1, .reusable = false}}, SHIFT(88),
  [61] = {.entry = {.count = 1, .reusable = true}}, SHIFT(88),
  [63] = {.entry = {.count = 1, .reusable = true}}, SHIFT(63),
  [65] = {.entry = {.count = 1, .reusable = false}}, SHIFT(43),
  [67] = {.entry = {.count = 1, .reusable = false}}, SHIFT(2),
  [69] = {.entry = {.count = 1, .reusable = true}}, SHIFT(2),
  [71] = {.entry = {.count = 1, .reusable = true}}, SHIFT(73),
  [73] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_block_repeat1, 2),
  [75] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_block_repeat1, 2), SHIFT_REPEAT(43),
  [78] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_block_repeat1, 2), SHIFT_REPEAT(2),
  [81] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_block_repeat1, 2), SHIFT_REPEAT(2),
  [84] = {.entry = {.count = 1, .reusable = true}}, SHIFT(60),
  [86] = {.entry = {.count = 1, .reusable = true}}, SHIFT(55),
  [88] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2),
  [90] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(122),
  [93] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(61),
  [96] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(124),
  [99] = {.entry = {.count = 1, .reusable = false}}, SHIFT(126),
  [101] = {.entry = {.count = 1, .reusable = true}}, SHIFT(74),
  [103] = {.entry = {.count = 1, .reusable = false}}, SHIFT(54),
  [105] = {.entry = {.count = 1, .reusable = false}}, SHIFT(132),
  [107] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(126),
  [110] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(54),
  [113] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(132),
  [116] = {.entry = {.count = 1, .reusable = true}}, SHIFT(77),
  [118] = {.entry = {.count = 1, .reusable = true}}, SHIFT(69),
  [120] = {.entry = {.count = 1, .reusable = true}}, SHIFT(67),
  [122] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 1),
  [124] = {.entry = {.count = 1, .reusable = true}}, SHIFT(3),
  [126] = {.entry = {.count = 1, .reusable = false}}, REDUCE(aux_sym_simple_path_repeat1, 2),
  [128] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_simple_path_repeat1, 2),
  [130] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_simple_path_repeat1, 2), SHIFT_REPEAT(115),
  [133] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_simple_path, 1),
  [135] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_simple_path, 1),
  [137] = {.entry = {.count = 1, .reusable = true}}, SHIFT(112),
  [139] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_use_group, 8),
  [141] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_use_group, 8),
  [143] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_simple_path_repeat1, 2), SHIFT_REPEAT(114),
  [146] = {.entry = {.count = 1, .reusable = true}}, SHIFT(108),
  [148] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_simple_path, 2),
  [150] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_simple_path, 2),
  [152] = {.entry = {.count = 1, .reusable = true}}, SHIFT(99),
  [154] = {.entry = {.count = 1, .reusable = true}}, SHIFT(101),
  [156] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_use_group, 5),
  [158] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_use_group, 5),
  [160] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_use_group, 6),
  [162] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_use_group, 6),
  [164] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_use_group, 7),
  [166] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_use_group, 7),
  [168] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_use_path, 1),
  [170] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_use_path, 1),
  [172] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_use_statement, 2),
  [174] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_use_statement, 2),
  [176] = {.entry = {.count = 1, .reusable = true}}, SHIFT(71),
  [178] = {.entry = {.count = 1, .reusable = true}}, SHIFT(68),
  [180] = {.entry = {.count = 1, .reusable = true}}, SHIFT(44),
  [182] = {.entry = {.count = 1, .reusable = true}}, SHIFT(76),
  [184] = {.entry = {.count = 1, .reusable = true}}, SHIFT(45),
  [186] = {.entry = {.count = 1, .reusable = true}}, SHIFT(33),
  [188] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_block, 3),
  [190] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_block, 3),
  [192] = {.entry = {.count = 1, .reusable = true}}, SHIFT(38),
  [194] = {.entry = {.count = 1, .reusable = true}}, SHIFT(49),
  [196] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_procedure_declaration, 8, .production_id = 4),
  [198] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_procedure_declaration, 8, .production_id = 4),
  [200] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_procedure_declaration, 7, .production_id = 3),
  [202] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_procedure_declaration, 7, .production_id = 3),
  [204] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_block, 2),
  [206] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_block, 2),
  [208] = {.entry = {.count = 1, .reusable = true}}, SHIFT(30),
  [210] = {.entry = {.count = 1, .reusable = true}}, SHIFT(31),
  [212] = {.entry = {.count = 1, .reusable = true}}, SHIFT(37),
  [214] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_module_declaration, 5, .production_id = 1),
  [216] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_module_declaration, 5, .production_id = 1),
  [218] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_use_statement, 3),
  [220] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_use_statement, 3),
  [222] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_module_declaration, 4, .production_id = 1),
  [224] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_module_declaration, 4, .production_id = 1),
  [226] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_expression_statement, 2),
  [228] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_expression_statement, 2),
  [230] = {.entry = {.count = 1, .reusable = true}}, SHIFT(125),
  [232] = {.entry = {.count = 1, .reusable = true}}, SHIFT(129),
  [234] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_return_statement, 3),
  [236] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_return_statement, 3),
  [238] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_use_item, 1),
  [240] = {.entry = {.count = 1, .reusable = true}}, SHIFT(127),
  [242] = {.entry = {.count = 1, .reusable = true}}, SHIFT(47),
  [244] = {.entry = {.count = 1, .reusable = true}}, SHIFT(52),
  [246] = {.entry = {.count = 1, .reusable = true}}, SHIFT(62),
  [248] = {.entry = {.count = 1, .reusable = true}}, SHIFT(56),
  [250] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_argument_list, 2),
  [252] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_argument_list_repeat1, 2), SHIFT_REPEAT(40),
  [255] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_type, 1),
  [257] = {.entry = {.count = 1, .reusable = true}}, SHIFT(57),
  [259] = {.entry = {.count = 1, .reusable = true}}, SHIFT(53),
  [261] = {.entry = {.count = 1, .reusable = true}}, SHIFT(36),
  [263] = {.entry = {.count = 1, .reusable = true}}, SHIFT(65),
  [265] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_parameter_list, 1),
  [267] = {.entry = {.count = 1, .reusable = true}}, SHIFT(102),
  [269] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_parameter_list, 2),
  [271] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_parameter_list_repeat1, 2),
  [273] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_parameter_list_repeat1, 2), SHIFT_REPEAT(102),
  [276] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_use_group_repeat1, 2),
  [278] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_use_group_repeat1, 2), SHIFT_REPEAT(98),
  [281] = {.entry = {.count = 1, .reusable = true}}, SHIFT(84),
  [283] = {.entry = {.count = 1, .reusable = true}}, SHIFT(42),
  [285] = {.entry = {.count = 1, .reusable = true}}, SHIFT(93),
  [287] = {.entry = {.count = 1, .reusable = true}}, SHIFT(41),
  [289] = {.entry = {.count = 1, .reusable = true}}, SHIFT(12),
  [291] = {.entry = {.count = 1, .reusable = true}}, SHIFT(18),
  [293] = {.entry = {.count = 1, .reusable = true}}, SHIFT(85),
  [295] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_parameter, 3, .production_id = 2),
  [297] = {.entry = {.count = 1, .reusable = true}}, SHIFT(87),
  [299] = {.entry = {.count = 1, .reusable = true}}, SHIFT(4),
  [301] = {.entry = {.count = 1, .reusable = true}}, SHIFT(17),
  [303] = {.entry = {.count = 1, .reusable = true}}, SHIFT(22),
  [305] = {.entry = {.count = 1, .reusable = true}}, SHIFT(116),
  [307] = {.entry = {.count = 1, .reusable = true}}, SHIFT(6),
  [309] = {.entry = {.count = 1, .reusable = true}},  ACCEPT_INPUT(),
  [311] = {.entry = {.count = 1, .reusable = true}}, SHIFT(25),
  [313] = {.entry = {.count = 1, .reusable = true}}, SHIFT(117),
  [315] = {.entry = {.count = 1, .reusable = true}}, SHIFT(72),
  [317] = {.entry = {.count = 1, .reusable = true}}, SHIFT(123),
  [319] = {.entry = {.count = 1, .reusable = true}}, SHIFT(11),
  [321] = {.entry = {.count = 1, .reusable = true}}, SHIFT(121),
  [323] = {.entry = {.count = 1, .reusable = true}}, SHIFT(20),
  [325] = {.entry = {.count = 1, .reusable = true}}, SHIFT(13),
  [327] = {.entry = {.count = 1, .reusable = true}}, SHIFT(15),
  [329] = {.entry = {.count = 1, .reusable = true}}, SHIFT(128),
  [331] = {.entry = {.count = 1, .reusable = true}}, SHIFT(78),
  [333] = {.entry = {.count = 1, .reusable = true}}, SHIFT(131),
};

#ifdef __cplusplus
extern "C" {
#endif
#ifdef _WIN32
#define extern __declspec(dllexport)
#endif

extern const TSLanguage *tree_sitter_xiulang(void) {
  static const TSLanguage language = {
    .version = LANGUAGE_VERSION,
    .symbol_count = SYMBOL_COUNT,
    .alias_count = ALIAS_COUNT,
    .token_count = TOKEN_COUNT,
    .external_token_count = EXTERNAL_TOKEN_COUNT,
    .state_count = STATE_COUNT,
    .large_state_count = LARGE_STATE_COUNT,
    .production_id_count = PRODUCTION_ID_COUNT,
    .field_count = FIELD_COUNT,
    .max_alias_sequence_length = MAX_ALIAS_SEQUENCE_LENGTH,
    .parse_table = &ts_parse_table[0][0],
    .small_parse_table = ts_small_parse_table,
    .small_parse_table_map = ts_small_parse_table_map,
    .parse_actions = ts_parse_actions,
    .symbol_names = ts_symbol_names,
    .field_names = ts_field_names,
    .field_map_slices = ts_field_map_slices,
    .field_map_entries = ts_field_map_entries,
    .symbol_metadata = ts_symbol_metadata,
    .public_symbol_map = ts_symbol_map,
    .alias_map = ts_non_terminal_alias_map,
    .alias_sequences = &ts_alias_sequences[0][0],
    .lex_modes = ts_lex_modes,
    .lex_fn = ts_lex,
    .primary_state_ids = ts_primary_state_ids,
  };
  return &language;
}
#ifdef __cplusplus
}
#endif
