#!/usr/bin/env python
import os


def WriteFile():
    NameFile="MachineFile.txt"
    NNodes = len(NodeList)
    if NNodes == 0:
        print("\n\tNo more nodes available\n\tReturn\n\n")
        return

    with open(NameFile, 'w') as f:
        for i in range(NNodes):
            towrite = NodeList[i].strip()
            towrite += " slots=36\n"
            f.write(towrite)

if __name__ == '__main__':
    nf = os.getenv("PSB_NODEFILE")
    print(nf)
    NodeList = []
    with open(nf, 'r') as f:
        
        for line in f:
            NodeList.append(line)

        NodeList = list(set(NodeList))
    print(NodeList)
    WriteFile()

