import sys

if __name__ == '__main__':
    raw = sys.argv[1]
    out = sys.argv[2]
    top = int(sys.argv[3])
    
    fid = open(out, 'w')
    with open(raw, 'r') as f:
        cnt = 0
        for line in f:
            cnt += 1
            if cnt > top:
                break
            fid.write('>a%d\n' % cnt)
            fid.write(line.strip() + '\n')
    fid.close()
