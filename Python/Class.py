import numpy as np
from sklearn.neighbors import KNeighborsClassifier
from sklearn import metrics

dach_length = [77,78,85,83,73,77,73,80]
dach_height = [25,28,19,30,21,22,17,35]

samo_length = [75,77,86,86,79,83,83,88]
samo_height = [56,57,50,53,60,53,49,61]

d_data = np.column_stack((dach_length, dach_height))
d_label = np.zeros(len(d_data))
s_data = np.column_stack((samo_length, samo_height))
s_label = np.ones(len(s_data))

newdata = [[79,35]]

dogs = np.concatenate((d_data,s_data))
labels = np.concatenate((d_label,s_label))

dog_classes = {0:'Dachshund',1:'Samoyed'}

k=3
knn = KNeighborsClassifier(n_neighbors = k)
knn.fit(dogs, labels)
y_pred = knn.predict(newdata)
print('데이터',newdata,',판정결과:',dog_classes[y_pred[0]])
