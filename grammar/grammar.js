module.exports = grammar({
  name: 'xiulang',

  rules: {
    source_file: $ => repeat($._item),

    _item: $ => choice(
      $.module_declaration,
      $.procedure_declaration,
      $.use_statement,
    ),

    module_declaration: $ => seq(
      'mod',
      field('name', $.identifier),
      '{',
      repeat($._item),
      '}'
    ),

    procedure_declaration: $ => seq(
      field('name', $.identifier),
      '::',
      '(',
      optional($.parameter_list),
      ')',
      '->',
      field('return_type', $.type),
      $.block
    ),

    parameter_list: $ => seq(
      $.parameter,
      repeat(seq(',', $.parameter))
    ),

    parameter: $ => seq(
      field('name', $.identifier),
      ':',
      field('type', $.type)
    ),

    use_statement: $ => seq(
      'use',
      $.use_path,
      optional(';')
    ),

    use_path: $ => choice(
      $.simple_path,
      $.use_group
    ),

    simple_path: $ => seq(
      $.identifier,
      repeat(seq('.', $.identifier))
    ),

    use_group: $ => seq(
      $.identifier,
      repeat(seq('.', $.identifier)),
      '.',
      '{',
      seq(
        $.use_item,
        repeat(seq(',', $.use_item)),
        optional(',')
      ),
      '}'
    ),

    use_item: $ => choice(
      $.identifier,
      $.use_group
    ),

    block: $ => seq(
      '{',
      repeat($._statement),
      '}'
    ),

    _statement: $ => choice(
      $.return_statement,
      $.expression_statement,
    ),

    return_statement: $ => seq(
      'return',
      $.expression,
      optional(';')
    ),

    expression_statement: $ => seq(
      $.expression,
      optional(';')
    ),

    expression: $ => choice(
      $.binary_expression,
      $.call_expression,
      $.member_expression,
      $.identifier,
      $.number,
    ),

    binary_expression: $ => prec.left(1, seq(
      field('left', $.expression),
      field('operator', choice('+', '-', '*', '/', '==', '!=', '<', '>', '<=', '>=')),
      field('right', $.expression)
    )),

    call_expression: $ => seq(
      field('function', $.expression),
      '(',
      optional($.argument_list),
      ')'
    ),

    argument_list: $ => seq(
      $.expression,
      repeat(seq(',', $.expression))
    ),

    member_expression: $ => prec.left(2, seq(
      field('object', $.expression),
      '.',
      field('property', $.identifier)
    )),

    type: $ => choice(
      's32',
      'u32',
      's64',
      'u64',
      'f32',
      'f64',
      'bool',
      'void',
      'char',
      'str`',
      $.identifier, 
    ),

    identifier: $ => /[a-zA-Z_][a-zA-Z0-9_]*/,

    number: $ => /\d+/,

    comment: $ => token(choice(
      seq('//', /.*/),
      seq('/*', /[^*]*\*+([^/*][^*]*\*+)*/, '/')
    )),
  },

  extras: $ => [
    /\s/,
    $.comment,
  ],

  conflicts: $ => [
    [$.use_path, $.simple_path],
  ],
});
