"""
Style for fit parameters / regions
"""
# __________________________________________________________________________
# alpha systematics

alpha_names = {
    't':r'tag $\tau$',
    'b':r'tag $b$',
    'c':r'tag $c$',
    'u': 'tag light',
    'jes':'JES',
    'jer':'JER',
    'el':r'$e$ ID',
    'mu':r'$\mu$ ID',
    'mscale':r'$\mu$ scale',
    'egzee':r'$e\,Z \to ee$',
    'eglow':r'$e$ low',
    'leptrig':r'$\ell$ trig',
    'met':r'$E_{\rm T}^{\rm miss}$ scale',
    'metres':r'$E_{\rm T}^{\rm miss}$ res',
    'mettrig':r'$E_{\rm T}^{\rm miss}$ trig',
    'topTheory': 'top theory',
    'WTheory': r'$W$ theory',
    'ZTheory': r'$Z$ theory',
    'signal_isr': 'signal isr',

    'jcb': r'Close By',
    'jbjes': r'$b$ JES',
    'jflavcomp': 'Flavor Comp',
    'jflavresp': 'Flavor Resp',
    'jicalm': r'$\eta$ Intcal Model',
    'jicals': r'$\eta$ Intcal Stat',
    'jpumu': r'Pileup $\mu$',
    'jpunpv': r'Pileup NPV',
    'jpupt': r'Pileup $p_{\rm T}$',
    'jpurho': r'Pileup $\rho$ Topo',
    'jsp': r'High $p_{\rm T}$ SP',
    }
for iii in range(6):
    np = iii + 1
    alpha_names['jenp{}'.format(np)] = 'Eff NP {}'.format(np)

# _________________________________________________________________________
# longer alpha names

def _tag(stem):
    return r'$\epsilon_{{\text{{tag}}}}$ for {}-jets'.format(stem)

long_alpha_names = {
    't': _tag(r'$\tau$'),
    'b': _tag(r'$b$'),
    'c': _tag(r'$c$'),
    'u': _tag('light'),
    'jes':'jet energy scale',
    'jer':'jet energy resolution',
    'el':r'$e$ ID SF',
    'mu':r'$\mu$ ID SF',
    'mscale':r'$\mu$ energy scale',
    'egzee':r'$e$ resolution from $Z \to ee$',
    'eglow':r'$e$ low $p_{\rm T}$',
    'leptrig':r'lepton trigger SF',
    'met':r'$E_{\rm T}^{\rm miss}$ scale',
    'metres':r'$E_{\rm T}^{\rm miss}$ resolution',
    'mettrig':r'$E_{\rm T}^{\rm miss}$ trigger SF',
    'topTheory': 'top theory',
    'WTheory': r'$W$ theory',
    'ZTheory': r'$Z$ theory',
    'signal_isr': 'signal ISR',

    'jcb': r'Close By',
    'jbjes': r'$b$ JES',
    'jflavcomp': 'Flavor Comp',
    'jflavresp': 'Flavor Resp',
    'jicalm': r'$\eta$ Intcal Model',
    'jicals': r'$\eta$ Intcal Stat',
    'jpumu': r'Pileup $\mu$',
    'jpunpv': r'Pileup NPV',
    'jpupt': r'Pileup $p_{\rm T}$',
    'jpurho': r'Pileup $\rho$ Topo',
    'jsp': r'High $p_{\rm T}$ SP',
    }
for iii in range(6):
    np = iii + 1
    long_alpha_names['jenp{}'.format(np)] = 'Eff NP {}'.format(np)

# __________________________________________________________________________
# mu parameters

mu_names = {
    'Wjets': r'$W$ + jets normalization',
    'Zjets': r'$Z$ + jets normalization',
    'top': r'top normalization',
    }

# ___________________________________________________________________________
# regions

reg_names = {
    'cr_t': 'CRT',
    'cr_w_mct150': 'CRW', 'cr_w': 'CRW',
    'signal_mct150': r'SR ($m_{\rm CT} > 150$)',
    'signal_mct200': r'SR ($m_{\rm CT} > 200$)',
    'signal_mct250': r'SR ($m_{\rm CT} > 250$)',
    'vr_mct': r'VR $m_{\rm CT} < 150$',
    'vr_mcc': r'VR $m_{cc} < 200$',
    'cr_w_mct150_l1pt50': r'CRW ($p_{\rm T}^{\ell_{1}} > 50$)',
    'cr_z': 'CRZ'}

