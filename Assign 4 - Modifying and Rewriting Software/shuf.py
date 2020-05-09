#!/usr/bin/python

import argparse
import random
import sys

class Shuffle:
    def __init__(self, evaluate, repeat, number):
        random.shuffle(evaluate)
        self.lines = evaluate
        self.repeat = repeat
        self.number = int(number)

    def printContents(self):
        if self.repeat == True and self.number is None:
            while True:
                print(random.choice(self.lines))
        elif self.repeat == True and self.number is not None:
            while self.number != 0:
                print(random.choice(self.lines))
                self.number-=1
        elif self.repeat == False and self.number is not None:
            count = int(len(self.lines))
            if int(self.number) < count:
                count = int(self.number)
            for line in self.lines:
                if count != 0:
                    print(line)
                else:
                    break
                count-=1
        else:
            for line in self.lines:
                print(line)
def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... """    
    lines = []
    if args.string_range is not None and args.can_repeat == True and args.num_lines != -1 and args.filename is not None:
        parser.error("Segmentation Fault")
    if args.string_range is not None:
        if args.filename is not None:
            parser.error("Segmentation Fault")
        begin, end = args.string_range.split('-', maxsplit = 1)

        if not begin.isdigit() or not end.isdigit():
            parser.error("invalid input range '" + args.string_range + "'")
        
        begin = int(begin)
        end = int(end)
        if begin > end:
            parser.error("invalid input range '" + args.string_range + "'")
       
        while (begin < end+1):
            lines.append(str(begin))
            begin+=1
    elif not len(sys.argv)> 1 or args.read_input == True or (args.filename is None and args.num_lines != -1 and args.can_repeat == True and args.string_range is None) or (args.num_lines != -1 and args.can_repeat == False and args.string_range is None and args.filename is None) or (args.num_lines == -1 and args.can_repeat == True and args.string_range is None and args.filename is None):
        lines = sys.stdin.readlines()
    else:
        f = open(args.filename, 'r') # Opens the file to be read      
        lines = f.readlines() # Read the file and store in a list          
        f.close()
    linesWithoutNewLines = []
    for line in lines:
        if line != '\n':
            linesWithoutNewLines.append(line.strip())
    shuffle = Shuffle(linesWithoutNewLines, args.can_repeat, args.num_lines)
    shuffle.printContents()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('filename', nargs='?')
    parser.add_argument("-", action="store_true", dest="read_input")

    parser.add_argument("-i", "--input-range", action="store", dest="string_range", default=None, help="treat each number LO through HI as an input line")

    parser.add_argument("-n", "--head-count", action="store", dest="num_lines", default=-1, help="output at most COUNT lines")

    parser.add_argument("-r", "--repeat", action="store_true", dest="can_repeat", default=False, help="output lines can be repeated")
    args = parser.parse_args(sys.argv[1:])

    main()



