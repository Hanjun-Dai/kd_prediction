import sys
import os

if __name__ == '__main__':
    field = sys.argv[1]

    output_folder = 'final_tune_%s' % field
    if not os.path.isdir(output_folder):
        os.makedirs(output_folder)

    d = {}
    d['nonlinear'] = ['0', '1']
    d['CONV_SIZE'] = ['32', '64', '128']
    d['LV'] = ['2', '3', '4', '5']
    d['bsize'] = ['16', '32', '64', '128']
    files = os.listdir('final_tune')
    for fname in files:
        content = []
        with open('final_tune/%s' % fname, 'r') as f:
            for line in f:
                line = line.rstrip()
                if 'RESULT_ROOT' in line:
                    line += '/%s' % field
                if 'save_dir=' in line:
                    line += '-nonlinear-$nonlinear'
                content.append(line)
                if 'max_pool=' in line:
                    content.append('nonlinear=0')
                if 'build/$tool' in line:
                    content.append('\t      -nonlinear $nonlinear')
                    
        for v in d[field]:
            with open('%s/%s-%s-%s' % (output_folder, field, v, fname), 'w') as f:
                for row in content:
                    if '%s=' % field in row:
                        f.write('%s=%s\n' % (field, v))
                    else:
                        f.write('%s\n' % row)