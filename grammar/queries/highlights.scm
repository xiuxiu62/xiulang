; Keywords
["mod" "use" "return"] @keyword

; Types
(primitive_type) @type.builtin
(custom_type) @type

; Functions
(procedure_declaration name: (identifier) @function)
(call_expression function: (identifier) @function.call)

; Identifiers
(identifier) @variable

; Literals
(number_literal) @number

; Operators
["+" "-" "*" "/" "%" "::"] @operator
