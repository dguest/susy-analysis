"""trying to migrate some of the susy-meta-lookup functions here"""
import yaml

_pkey = 'preferred'
def find_preferred(name, pref_file_name):
    with open(name) as yml:
        file_dict = yaml.load(yml)
    preferred_datasets = set()
    with open(pref_file_name) as pref:
        for line in pref:
            ds_name = line.strip().rstrip('/')
            preferred_datasets.add(ds_name)

    # modify the meta here
    new_meta = {}
    for key, dsmeta in file_dict.iteritems():
        full_name = dsmeta['full_name']
        was_preferred = dsmeta.get(_pkey, False)
        preferred = full_name in preferred_datasets
        if preferred:
            dsmeta[_pkey] = True
        elif was_preferred:
            del dsmeta[_pkey]
        new_meta[key] = dsmeta
    file_dict.update(new_meta)

    # write out the new file
    with open(name, 'w') as out:
        out.write(yaml.dump(file_dict))
