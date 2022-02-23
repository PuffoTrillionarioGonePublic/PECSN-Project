from collections import defaultdict
import itertools as it
import pandas as pd
import numpy as  np
import matplotlib.pyplot as plt

def read_stats(file, *params):
    
    usecols=["type", "name", "value", "count", "mean", "stddev", "min", "max"]

    rv = defaultdict(dict)
    for par in it.product(*params):
        df = pd.read_csv(file.format(*par), usecols=usecols)

        df = df[(df.type == "scalar") | (df.type == "statistic")]
        df["name"] = df["name"].map(lambda n: n.split(":")[0])
        df = df.groupby("name").sum()
        df = df.rename(columns={'value': 'timeavg'})

        for name, v in zip(df.index, df[["timeavg", "count", "mean", "stddev", "min", "max"]].itertuples(index=False)):
            rv[name][par] = v
    return rv

def read_vectors(file, *params):

    def parse_ndarray(s):
        return np.fromstring(s, sep=' ') if s else None
    
    def squeezets(t, x):
        nt, nx = [], []
        for ti, tip1, xi in it.zip_longest(t, t[1:], x, fillvalue=None):
            if ti != tip1:
                nt.append(ti)
                nx.append(xi)
        return np.asarray(nt), np.asarray(nx)
    
    converters = {
      'vectime': parse_ndarray,
      'vecvalue': parse_ndarray
    }

    usecols=["type", "name", "vectime", "vecvalue"]

    rv = defaultdict(dict)
    for par in it.product(*params):
        df = pd.read_csv(file.format(*par), converters=converters, usecols=usecols)

        df = df[df.type == "vector"]
        df["name"] = df["name"].map(lambda n: n.split(":")[0])
        squeezed = df[["vectime", "vecvalue"]].apply(lambda x:squeezets(x.vectime, x.vecvalue), axis=1, result_type='expand')
        df[["vectime", "vecvalue"]] = squeezed

        for name, v in zip(df["name"], df[["vectime", "vecvalue"]].itertuples(index=False)):
            rv[name][par] = v
    return rv

def twok(factors, measurements):
	""" 
	factors: list of identifiers
	measurements[idx] = measurement obtained with factors[i] high if idx[i] is +1 else low and idx[i] = -1"
	"""
	
	k = len(factors)

	main_table = list(it.product(*it.repeat([-1, +1], k)))
	interactions = list(it.chain.from_iterable(it.combinations(range(k), i) for i in range(2, k+1)))
	matrix = [(1, *m , *(np.prod([m[i] for i in idx]) for idx in interactions)) for m in main_table]

	y = [measurements[m] for m in main_table]
	qi = np.linalg.solve(matrix, y)
	qi = np.delete(qi, 0)

	SSx = qi**2 * 2**k
	SST = np.sum(SSx)
	fx = SSx/SST

	names = [" & ".join(factors[i] for i in idx) for idx in it.chain([[r] for r in range(k)], interactions)] 
	fx_w_names = {n:v for n,v in zip(names, fx)}
	return fx_w_names

def twokr(factors, measurements):
	""" 
	factors: list of identifiers
	measurements[idx] = list of measurement obtained with factors[i] high if idx[i] is +1 else low and idx[i] = -1"
	"""
	
	k = len(factors)
	r = len(next(v for v in measurements.values()))

	main_table = list(it.product(*it.repeat([-1, +1], k)))
	interactions = list(it.chain.from_iterable(it.combinations(range(k), i) for i in range(2, k+1)))
	matrix = [(1, *m , *(np.prod([m[i] for i in idx]) for idx in interactions)) for m in main_table]

	y = [np.mean(measurements[m]) for m in main_table]
	qi = np.linalg.solve(matrix, y)
	qi = np.delete(qi, 0) 

	error_matrix = np.asarray([np.asarray(v) - np.mean(v) for v in measurements.values()])
	SSE = np.sum(error_matrix**2)

	SSx = qi**2 * r * 2**k
	SSx = np.append(SSx, SSE)
	SST = np.sum(SSx)
	fx = SSx/SST

	names = [" & ".join(factors[i] for i in idx) for idx in it.chain([[r] for r in range(k)], interactions)] 
	names.append("SSE/SST")
	fx_w_names = {n:v for n,v in zip(names, fx)}
	return fx_w_names

def test():
	# 1
	factors = ["A", "B"]
	measurements = {(-1,-1):15, (1, -1):45, (-1, 1):25, (1, 1):75}
	assert twok(factors, measurements) == {'A': 0.7619047619047619, 'B': 0.19047619047619047, 'A & B': 0.047619047619047616}

	# 2
	factors = ["A", "B", "C"]
	measurements = {
		(-1, -1, -1):14,
		(+1, -1, -1):22,
		(-1, +1, -1):10,
		(+1, +1, -1):34,
		(-1, -1, +1):46,
		(+1, -1, +1):58,
		(-1, +1, +1):50,
		(+1, +1, +1):86
		}
	assert twok(factors, measurements) == {'A': 0.1773049645390071, 'B': 0.044326241134751775, 'C': 0.7092198581560284, 'A & B': 0.044326241134751775, 'A & C': 0.0070921985815602835, 'B & C': 0.015957446808510637, 'A & B & C': 0.0017730496453900709}

	# 3
	factors = ["A", "B"]
	measurements = {(-1,-1):[15, 18, 12], (1, -1):[45, 48, 51], (-1, 1):[25, 28, 19], (1, 1):[75, 75, 81]}
	assert twokr(factors, measurements) == {'A': 0.7888225255972696, 'B': 0.15401023890784982, 'A & B': 0.042662116040955635, 'SSE/SST': 0.014505119453924915}

def plot_confidence_interval(x, values, z=1.96, color='#2187bb', horizontal_line_width=0.01):
    mean = np.mean(values)
    stdev = np.std(values)
    confidence_interval = z * stdev / np.sqrt(len(values))

    left = x - horizontal_line_width / 2
    top = mean - confidence_interval
    right = x + horizontal_line_width / 2
    bottom = mean + confidence_interval
    plt.plot([x, x], [top, bottom], color=color)
    plt.plot([left, right], [top, top], color=color)
    plt.plot([left, right], [bottom, bottom], color=color)
    plt.plot(x, mean, 'o', color=color)

    return mean, confidence_interval