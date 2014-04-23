_ttbar_samples = [117050]  #117050 or 105861
_singleTop_samples = [110101, 110119, 110140]

wills_samples = dict(
    ttbar_samples = _ttbar_samples,
    singleTop_samples = _singleTop_samples,
    top_samples = _ttbar_samples + _singleTop_samples ,
    Wjets_samples = (
        range(167740,167748+1) +
        range(167761,167796+1) +
        range(180534,180542+1) ),
    WjetsB_samples = (
        range(167740,167748+1,3) +
        range(167761,167796+1,3) +
        range(180534,180542+1,3) ),
    WjetsC_samples = (
        range(167741,167748+1,3) +
        range(167762,167796+1,3) +
        range(180535,180542+1,3) ),
    WjetsL_samples = (
        range(167742,167748+1,3) +
        range(167763,167796+1,3) +
        range(180536,180542+1,3) ),
    Zjets_samples = (
        range(167749,167760+1) +
        range(167797,167844+1) +
        range(180543,180551+1) ),
    ZjetsB_samples = (
        range(167749,167760+1,3) +
        range(167797,167844+1,3) +
        range(180543,180551+1,3) ),
    ZjetsC_samples = (
        range(167750,167760+1,3) +
        range(167798,167844+1,3) +
        range(180544,180551+1,3) ),
    ZjetsL_samples = (
        range(167751,167760+1,3) +
        range(167799,167844+1,3) +
        range(180545,180551+1,3) ),
    QCD_samples  = range(147910,147917+1),
    diboson_samples = (
        [177997,177999,179974,179975] +
        range(183585,183592+1) +
        range(183734,183739+1)),
    ttbarV_samples = [119353, 119355] + range(174830,174833+1),
    )

_scharm_ext = range(186635,186671+1)
_scharm_stop_xcheck = range(186641,186643+1)
_scharm_ext_no_xchk = [x for x in _scharm_ext if x not in _scharm_stop_xcheck]

scharm = {
    'scharm-signal': range(183424,183474+1),
    'scharm-signal-ext': _scharm_ext_no_xchk
    }

variations = {
    # truth level... maybe not for now
    # 'ttbar_scale': DsRange( (174763, 174766)),
    'ttbar_parton_shower_jimmy': [105860],
    'ttbar_parton_shower_pythia': [105861],
    'ttbar_isr_fsr_more': [117209],
    'ttbar_isr_fsr_less': [117210],
    'ttbar_mcnlo': [105200],
    # 'ttbar_alpgen': DsRange((164440,164443), (164450, 164453)) + [
    #     116108, 116109],
    'ttbar_sherpa': range(117800, 117809+1),
    # 'ttbar_powheg_pythia':[117050],
    }

