import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
from scipy import signal

rdms = [0, 1]

fig, axes = plt.subplots(len(rdms), 2)

w0, yrs = 52*0, 5
weeks_to_plot = range(w0, w0 + 52 * yrs)

Obwf = 2
Sf = 1
cf = 2. / 52.178

for rdm in range(len(rdms)):
    demands = np.loadtxt('rdm_{}/demands/raleigh_demand.csv'.format(rdm), delimiter=',')
    inflows = np.loadtxt('rdm_{}/inflows/falls_lake_inflows.csv'.format(rdm), delimiter=',')[:, range(2609, 2609 + 52*48)]

    for r in range(1):
        infs = inflows[r, weeks_to_plot]
        dems = demands[r, weeks_to_plot]

        b, a = signal.butter(Obwf, cf, 'low')
        filtered_infs = signal.filtfilt(b, a, infs)
        filtered_dems = signal.filtfilt(b, a, dems)

        axes[0, rdm].plot(infs , c=cm.get_cmap('tab10').colors[r], alpha=0.5)
        axes[1, rdm].plot(dems, c=cm.get_cmap('tab10').colors[r], alpha=0.5)
        axes[0, rdm].plot(filtered_infs , c=cm.get_cmap('tab10').colors[r], lw=2)
        axes[1, rdm].plot(filtered_dems, c=cm.get_cmap('tab10').colors[r], lw=2)
        axes[0, rdm].set_yscale('log')
        axes[0, rdm].grid(which='major', axis='x')
        axes[1, rdm].grid(which='major', axis='x')

plt.tight_layout()
plt.show()
