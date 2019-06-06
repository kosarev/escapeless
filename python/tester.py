#!/usr/bin/env python

from builtins import bytes
import sys

import escapeless


def error(msg):
    sys.exit('tester: %s' % msg)


def read_bytes():
    while True:
        line = sys.stdin.readline()
        if line == '':
            error('unexpected end of file')

        if line == '\n' or line.startswith('#'):
            continue

        line = line.rstrip()
        break

    b = []
    for group in line.split():
        group = group.split('..')
        if len(group) == 1:
            b.append(int(group[0], 16))
            continue

        begin, end = int(group[0], 16), int(group[1], 16)
        i = begin
        b.append(i)
        while True:
            i = (i + 1) % 0x100
            b.append(i)

            if i == end:
                break

    return bytes(b)


def check_end_of_file():
    while True:
        line = sys.stdin.readline()
        if line == '':
            break

        if line == '\n' or line.startswith('#'):
            continue

        line = line.rstrip()
        error('extra line: %r' % line)


def test_encoding(input, takeouts, encoded):
    block, takeouts_map = escapeless.encode(input, takeouts)

    output = takeouts_map + block
    if output != encoded:
        print('input', input)
        print('takeouts', takeouts)

        print('takeouts_map', takeouts_map)
        print('block', block)
        print('output', output)

        print('encoded', encoded)

        error("encoded block doesn't match")


def test_decoding(input, takeouts, encoded):
    takeouts_size = len(takeouts)
    takeouts_map = encoded[:takeouts_size]
    block = encoded[takeouts_size:]

    block = escapeless.decode(block, takeouts, takeouts_map)

    if block != input:
        print('input', input)
        print('takeouts', takeouts)

        print('takeouts_map', takeouts_map)
        print('block', block)

        error("decoded block doesn't match")


def test(input, takeouts, encoded):
    test_encoding(input, takeouts, encoded)
    test_decoding(input, takeouts, encoded)


def main():
    input = read_bytes()
    takeouts = read_bytes()
    encoded = read_bytes()
    check_end_of_file()

    test(input, takeouts, encoded)


if __name__ == "__main__":
    main()
