# Extended Keymaps for the Kaleidoscope Keyboard Firmware

This plugin features the following types of keymaps as a drop in replacement
for Kaleidoscope's traditional keymap definition.

* sparse (specify only those keys that differ from a given default key)
* stacked (same as Kaleidoscope's standard keymap definition)
* mirrored (references another keymap)
* shifted (references another keymap)
* shifted and wrapped (references another keymap)
* bitmasked keymaps (references another keymap; uses its keys only if corresponding bits in a keymask are non-zero)

Also, this plugin provides the possibility to 
define a layer tree instead of the traditional layer stack (that is also still supported for compatibility reasons).

## Terminology

The meaning of the term layer is slightly modified in the context of XKeymaps. A layer is now represented by either a node in the layer tree or by a level
of the layer stack. Layer ids remain zero based. The actual XKeymaps are defined independently from layers and are referenced via individual identifiers that are also zero based. This makes it possible to share a keymap reference between multiple layers. Thus, the meaning of a keymap depends upon from where in the layer tree it is referenced. 

When using a traditional layer stack, layer ids coincide with XKeymap ids.

## Usage

See the header file `Kaleidoscope-XKeymaps.h` for further documentation of the 
macros that are available for defining keymaps.

The following is a minimal example sketch.

```cpp
// Define a keymask (a bitfield wherein every bit represents one key) that is
// stored in PROGMEM. It is used further below for masking of keymaps.
//
KEYMASK_STACKED_PROGMEM(my_keymask, 
   0, 0, 0, 0, 0, 0, 0,
   0, 1, 1, 1, 1, 1, 0,
   0, 1, 1, 1, 1, 1,
   0, 0, 0, 0, 1, 1, 1,
            0, 1, 1, 0,
                     0,
                     
   0, 0, 0, 0, 0, 0, 0, 
   0, 1, 1, 1, 1, 1, 0, 
      1, 1, 1, 1, 1, 0, 
   1, 1, 1, 0, 0, 0, 0,
   0, 1, 1, 0,
   0
);

// Use keymap nicknames to avoid confusion with layer ids (tree node ids).
//
enum Keymaps { Base, Snap, Crackle, Pop, Fish, Cupid, Storm };

XKEYMAPS(XXX /* The fallback key */,
  // Stacked keymaps can be defined analogously to the way
  // they are traditionally defined in Kaleidoscope.
  //
  XKEYMAP_STACKED(Base, 
     ___,          Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext,
     Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
     Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
     Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,
     Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
     ShiftToLayer(FUNCTION),

     M(MACRO_ANY),  Key_6, Key_7, Key_8,     Key_9,         Key_0,         LockLayer(NUMPAD),
     Key_Enter,     Key_Y, Key_U, Key_I,     Key_O,         Key_P,         Key_Equals,
                    Key_H, Key_J, Key_K,     Key_L,         Key_Semicolon, Key_Quote,
     Key_RightAlt,  Key_N, Key_M, Key_Comma, Key_Period,    Key_Slash,     Key_Minus,
     Key_RightShift, Key_LeftAlt, Key_Spacebar, Key_RightControl,
     ShiftToLayer(FUNCTION) 
  )
  XKEYMAP_SPARSE(Snap, Key_Transparent /* the default key */,
     /* Important: Sparse entry must be listed in ascending order of
                   the result of row*COLS + col. */
     XKEYMAP_SPARSE_ENTRY(2, 3, Key_A),
     XKEYMAP_SPARSE_ENTRY(7, 8, Key_B)
  )
  XKEYMAP_MIRRORED(Crackle, Snap /* mirrors Snap */)
  XKEYMAP_SHIFTED(Pop, Snap /* shifts Snap */, XXX, 2 /* row-offset */, 3 /* col-offset */)
  XKEYMAP_MIRRORED(Fish, Pop /* mirrors the already shifted Pop */)
  XKEYMAP_SHIFTED_WRAPPED(Cupid, Snap /* shifts Snap */, 2 /* row-offset */, 3 /* col-offset */)
  XKEYMAP_MASKED(Storm, Pop, my_keymask, Key_Transparent /* the default key */, /* empty = don't negate */)
)

// When using a layer tree, layers are tree nodes which are each bound
// to a keymap. Keymaps can thereby be shared among nodes.
//
// 0(Base)---1(Snap)---5(Fish)
//         |
//         *-2(Crackle)---3(Pop)
//                      |
//                      *-4(Fish)
//
XKEYMAP_TREE(
   XKEYMAP_TREE_ROOT(Base),                                               /* node 0 */
   XKEYMAP_TREE_NODE(0 /* parent node */, Base    /* referenced slot */), /* node 1 */
   XKEYMAP_TREE_NODE(0 /* parent node */, Crackle /* referenced slot */), /* node 2 */
   XKEYMAP_TREE_NODE(2 /* parent node */, Pop     /* referenced slot */), /* node 3 */
   XKEYMAP_TREE_NODE(2 /* parent node */, Fish    /* referenced slot */), /* node 4 */
   XKEYMAP_TREE_NODE(1 /* parent node */, Fish    /* referenced slot */), /* node 5 */
)

// We could also use a traditional layer stack by replacing the above definition
// of XKEYMAP_TREE(...) by XKEYMAP_STACK

...

KALEIDOSCOPE_INIT_PLUGINS(XKeymaps)
```
