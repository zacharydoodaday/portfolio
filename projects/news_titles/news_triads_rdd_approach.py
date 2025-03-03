from pyspark import SparkContext, SparkConf
import sys
import re
from itertools import combinations
from operator import itemgetter

class Project2:   
        
    def run(self, inputPath, outputPath, stopwords, k):
        conf = SparkConf().setAppName("project2_rdd").setMaster("local")
        sc = SparkContext(conf=conf)
        
        # Create a broadcast variable for the stopwords
        stp = sc.textFile(stopwords)
        stpRDD = stp.flatMap(lambda x: x.split(" ")).collect()
        stopWords = sc.broadcast(stpRDD)

        # Open the input file and perform filters on the dataset
        text = sc.textFile(inputPath)
        textRDD = text.map(lambda line: line.split(",",1))
        mappedRDD = textRDD.mapValues(lambda values: re.split("[\\s*$&#/\"'\\,.:;?!\\[\\](){}<>~\\-_]+", values))
        filteredRDD = mappedRDD.mapValues(lambda values: [v.lower() for v in values if v.lower() not in stopWords.value and v.isalpha()])

        # Create a function to create triads
        def triads(values):
            return list(combinations(values, 3))
        
        triadCombsRDD = filteredRDD.flatMapValues(triads)

        # Alphabetically sort the terms within each triad
        triadValueSortedRDD = triadCombsRDD.mapValues(lambda x: sorted(x))

        # Alphabetically sort the categories
        triadKeySortedRDD = triadValueSortedRDD.sortBy(lambda x: ''.join(x[1])).sortByKey()

        # Create a count for each triad and reduce
        countedRDD = triadKeySortedRDD.mapValues(lambda x: (x, 1))
        compositeKeyRDD = countedRDD.map(lambda x: ((x[0], tuple(x[1][0])), x[1][1]))
        countedCompositeRDD = compositeKeyRDD.reduceByKey(lambda a,b: a + b)
        valuesSeparatedRDDFormat = countedCompositeRDD.map(lambda x: (x[0][0], list(x[0][1]), x[1]))
        
        # Create a separate RDD to obtain the number of invalid headlines per category
        invalidDictionaryRDD = filteredRDD.filter(lambda x: len(x[1]) < 3).map(lambda x: (x[0],1)).reduceByKey(lambda a,b: a + b).sortByKey()

        # Create a separate dictionary to obtain nCat (valid headlines) per category
        nCatDictionary = filteredRDD.filter(lambda x: len(x[1]) >= 3).map(lambda x: (x[0],1)).reduceByKey(lambda a,b: a + b).collectAsMap()

        # Calculate rfCat for each line
        def calcRF(category, value, nCatDictionary):
            return value / nCatDictionary[category]

        rfCatRDD = valuesSeparatedRDDFormat.map(lambda x: (x[0],x[1],calcRF(x[0],x[2],nCatDictionary)))
        
        # Only show the top k triads for each category
        k = int(k)
        topKPerCatRDD = (rfCatRDD.groupBy(lambda x: x[0]).mapValues(lambda values: sorted(values,key=itemgetter(2),reverse=True)[:k]))

        # Format the grouped data
        formattedGroupedDataRDD = topKPerCatRDD.flatMap(lambda x: [(x[0], f"{','.join(item[1])}:{item[2]}") for item in x[1]])

        def formatOutput(key, value):
            return f"{key}\t{value}"
    
        # Combine the invalid lines with the grouped data and prioritize invalid lines by using a dummy variable
        resultRDD = (invalidDictionaryRDD.map(lambda x: (x[0], f"invalid line:{x[1]}", 0))
                    .union(formattedGroupedDataRDD.map(lambda x: (x[0], x[1], 1)))
                    .sortBy(lambda x: (x[0], x[2])))
        resultantRDD = resultRDD.map(lambda x: formatOutput(x[0], x[1]))

        # This section is for output
        resultantRDD.coalesce(1).saveAsTextFile(outputPath)
        sc.stop()

if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("Wrong arguments")
        sys.exit(-1)
    Project2().run(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])

