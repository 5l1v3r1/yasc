from argparse import ArgumentParser, FileType
import sys
from typing import List


def encode(data: List[bytes], key: str) -> None:
    for ndx in range(len(data)):
        data[ndx] = data[ndx] ^ ord(key[ndx%len(key)])
    return

def parse_args(args) -> ArgumentParser.parse_args:
    parser = ArgumentParser("Encode a Stage 2 payload for solo.ranger")
    parser.add_argument("payload", metavar="INFILE", type=FileType('rb'), help="The payload to encode")
    parser.add_argument("outfile", metavar="OUTFILE", default="solo.ranger_stage2.packed", type=FileType('wb'), help="Output of encoded payload")
    parser.add_argument("--key", "-k", default="solo.ranger", help="The key to use to XOR the binary")
    # TODO Use pefile to verify payload is PE/COFF/ELF
    return parser.parse_args(args)

def main(options: ArgumentParser.parse_args):
    options.outfile.write(b"BM")
    while (data := bytearray(options.payload.read(16384))) != bytearray(b''):
        #encode(data, options.key)
        options.outfile.write(data)
    return


if __name__ == "__main__":
    main(parse_args(sys.argv[1:]))