"""
Style for fit parameters / regions
"""
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

reg_names = {
    'cr_t': 'CRT',
    'cr_w_mct150': 'CRW', 'cr_w': 'CRW',
    'signal_mct150': r'SR ($m_{\rm CT} > 150$)',
    'cr_w_mct150_l1pt50': r'CRW ($p_{\rm T}^{\ell_{1}} > 50$)',
    'cr_z': 'CRZ'}

