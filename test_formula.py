
from lotus.notes import *

s = Session()

formula = Formula()

formula.text = """
@round(234.55) + @tonumber(@text(@Uniqeasdouasdhasiuhdiuh));
@return(@prompt([ok]; "test"; "test"));
@asdoaudhs
"""

print """
Formula Test
------------"""

formula_strings =\
    ["@Unique",
     "@Now",
     "@Text('Hello')",
     "@ToNumber(3.14)",
     "@Return(@BrokenFormula)"]

for f in enumerate(formula_strings):
  formula = Formula(f[1])
  result = formula.eval()
  print """
Formula:          %s
Compiled size:    %i bytes
Result:           %s
Result type:      %s""" \
    % ( formula,
        result.value,
        formula.size,
        result.type() ) 

