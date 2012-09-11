import _cutflow as cf

def run_cutflow(input_file): 
    out_dic = cf._cutflow(input_file, 'v')
    for name, count in out_dic: 
        print name, count
