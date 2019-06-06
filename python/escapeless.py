#!/usr/bin/env python

from builtins import bytes


def encode(input, takeouts):
    # Find replacements for used takeout characters.
    used_chars = set(input)
    unused_chars = sorted(set(range(0, 0x100)) - used_chars - set(takeouts))
    takeouts_map = []
    chars_map = range(0, 0x100)
    for t in takeouts:
        if t in used_chars:
            c = unused_chars.pop(0)
            takeouts_map.append(c)
            chars_map[t] = c
        else:
            takeouts_map.append(None)

    # Map all unused takeouts to a (or the) remaining unused
    # character.
    c = unused_chars.pop(0)
    takeouts_map = [c if x is None else x for x in takeouts_map]

    # Replace takeouts in the input.
    block = bytes([chars_map[c] for c in input])

    return block, takeouts_map


def decode(block, takeouts, takeouts_map):
    # Build the restoring map.
    chars_map = range(0, 0x100)
    takeouts_map = list(takeouts_map)
    for t in takeouts:
        chars_map[takeouts_map.pop(0)] = t

    # Apply the map.
    block = bytes([chars_map[c] for c in block])

    return block
