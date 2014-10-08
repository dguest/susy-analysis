"""
Style for fit parameters / regions
"""
# __________________________________________________________________________
# general names

lsp = r'\tilde{\chi}_1^0'
scharm = r'\tilde{c}_1'
stop = r'\tilde{t}_1'

# __________________________________________________________________________
# alpha systematics

alpha_names = {
    't':r'tag $\tau$',
    'b':r'tag $b$',
    'c':r'tag $c$',
    'u': 'tag light',
    'jes':'JES',
    'jer':'JER',
    'pu': 'pileup',
    'jvf': 'JVF',
    'el':r'$e$ ID',
    'mu':r'$\mu$ ID',
    'mscale':r'$\mu$ scale',
    'egzee':r'$e\,Z \to ee$',
    'eglow':r'$e$ low',
    'leptrig':r'$\ell$ trig',
    'met':r'$E_{\rm T}^{\rm miss}$ scale',
    'metres':r'$E_{\rm T}^{\rm miss}$ res',
    'mettrig':r'$E_{\rm T}^{\rm miss}$ trig',
    'flat_other': "flat other",
    'theory_top': 'top theory',
    'theory_Wjets': r'$W$ theory',
    'theory_Zjets': r'$Z$ theory',
    'signal_isr': r'\tilde{c} ISR / FSR',

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
    'pu': 'pileup',
    'jvf': 'jet vertex fraction',
    'el':r'$e$ ID SF',
    'mu':r'$\mu$ ID SF',
    'mscale':r'$\mu$ energy scale',
    'egzee':r'$e$ resolution from $Z \to ee$',
    'eglow':r'$e$ low $p_{\rm T}$',
    'leptrig':r'lepton trigger SF',
    'met':r'$E_{\rm T}^{\rm miss}$ scale',
    'metres':r'$E_{\rm T}^{\rm miss}$ resolution',
    'mettrig':r'$E_{\rm T}^{\rm miss}$ trigger SF',
    'signal_isr': 'signal ISR',
    'flat_other': "flat ``other'' systematic",
    'theory_top': 'combined top theory',
    'theory_Wjets': r'combined $W$ theory',
    'theory_Zjets': r'combined $Z$ theory',
    'signal_isr': r'signal ISR / FSR',

    'jcb': r'Close By',
    'jbjes': r'$b$ JES',
    'jflavcomp': 'Flavor Composition',
    'jflavresp': 'Flavor Response',
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
    'Wjets': r'$W$ + jets',
    'Zjets': r'$Z$ + jets',
    'top': r'top',
    }

# ___________________________________________________________________________
# regions

reg_names = {
    'cr_t': 'CRT',
    'cr_w_mct150': 'CRW', 'cr_w': 'CRW',
    'signal_mct150': r'$m_{\rm CT} > 150$',
    'signal_mct200': r'$m_{\rm CT} > 200$',
    'signal_mct250': r'$m_{\rm CT} > 250$',
    'vr_mct': r'VR $m_{\rm CT} < 150$',
    'vr_mcc': r'VR $m_{cc} < 200$',
    'cr_w_mct150_l1pt50': r'CRW ($p_{\rm T}^{\ell_{1}} > 50$)',
    'cr_z': 'CRZ'}

# __________________________________________________________________________
# configurations

config_names = {
    'mct150': r'$m_{\sf CT} > 150$',
    'mct200': r'$m_{\sf CT} > 200$',
    'mct250': r'$m_{\sf CT} > 250$',
    'C1': r'${} \to c {}$: C1'.format(stop, lsp),
    'C2': r'${} \to c {}$: C2'.format(stop, lsp),
    'expected': r'Expected Limit ($\pm 1 \sigma^{\sf SM}_{\sf theory+exp}$)',
    'observed': r'Observed Limit ($\pm 1 \sigma^{\sf SUSY}_{\sf theory})$'
    }
