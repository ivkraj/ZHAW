For LTSpice simulation:
- copy the opa320.lib file into your LTSpice .. \lib\sub folder
- in the simulation sheet: 
  use the opamp2 symbol, edit its Value property to OPA320
  enter the spice directive (.op button from the menu ribbon): .include OPA320.lib
 