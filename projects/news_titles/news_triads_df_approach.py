from pyspark.sql.session import SparkSession
from pyspark.sql.functions import *
from pyspark.sql.types import *
import sys

import re
from itertools import combinations
from pyspark.sql.window import Window

class Project2:           
    def run(self, inputPath, outputPath, stopwords, k):
        spark = SparkSession.builder.master("local").appName("project2_df").getOrCreate()
        
        # Create two columns, one with the category and another with the headline
        fileDF = spark.read.text(inputPath)
        splitDF = fileDF.withColumn("category", expr("split(value, ',', 2)[0]")).withColumn("headline", expr("split(value, ',', 2)[1]"))
        baseDF = splitDF.select("category", "headline")

        # Create a global variable with the stopwords
        stopwordsDF = spark.read.text(stopwords)
        stopwordsList = [row.value for row in stopwordsDF.collect()]

        # Create a UDF that completes all filtering steps and apply it
        def strToArray(headline):
            returnList = []
            arrayHeadline = re.split("[\\s*$&#/\"'\\,.:;?!\\[\\](){}<>~\\-_]+", headline)
            for term in arrayHeadline:
                if term.lower() not in stopwordsList and term.isalpha():
                    returnList.append(term.lower())
            return returnList
        converterUDF = udf(lambda headline: strToArray(headline), ArrayType(StringType()))
        
        convertedDF = baseDF.withColumn("conv_hl", converterUDF("headline")).select("category", "conv_hl")

        # Create a separate DF that includes the count of invalid lines per category
        invDF = convertedDF.filter(size(col("conv_hl")) < 3).groupBy("category").count()

        # Create a separate DF that includes the count of nCat (valid lines) per category
        nCatDF = convertedDF.filter(size(col("conv_hl")) >= 3).groupBy("category").count().withColumnRenamed("count","nCat")

        # Create a UDF that gives us all the term triads using combinations and apply it
        def triads(headline):
            comb = list(combinations(headline, 3))
            return [','.join(terms) for terms in comb]
        triadsUDF = udf(lambda headline: triads(headline), ArrayType(StringType()))

        triadsDF = convertedDF.withColumn("triads", triadsUDF("conv_hl"))

        # Use the explode function to place each triad into its own row with its corresponding category
        explodedDF = triadsDF.withColumn("triad", explode("triads")).select("category", "triad")
        
        # Before counting, first rearrange each triad order alphabetically using a UDF and apply it
        def triadOrdering(triad):
            triadAsArray = triad.split(',')
            sortedTriads = sorted(triadAsArray)
            stringSortedTriads = ','.join(sortedTriads)
            return stringSortedTriads
        tOrderingUDF = udf(lambda triads: triadOrdering(triads), StringType())

        orderedTriadsDF = explodedDF.withColumn("oTriads", tOrderingUDF("triad")).select("category","oTriads").withColumnRenamed("oTriads","triad")
        
        # Count the number of lines with the same category and triad
        countDF = orderedTriadsDF.groupBy("category","triad").count()

        # Use the join function to include our nCat DF with our main DF to calculate each line's rfCat
        joinedDF = countDF.join(nCatDF, on="category", how="inner")
        computedDF = joinedDF.withColumn("adjustedCount", col("count") / col("nCat")).select("category","triad","adjustedCount")

        # Order all lines first by category, then count, then triad
        orderedDF = computedDF.orderBy("category", desc("adjustedCount"), "triad")

        # Obtain the top k values with the window term
        windowSpec = Window.partitionBy("category").orderBy(col("adjustedCount").desc())
        orderedDFWithRank = orderedDF.withColumn("rank", row_number().over(windowSpec))
        k = int(k)
        topKDF = orderedDFWithRank.filter(col("rank") <= k)

        # Format the output by first preparing the topKDF and invDF separately with dummy variables
        joinDF = topKDF.join(invDF, on="category", how="left")
        formattedTopKDF = topKDF.withColumn("formatted", concat_ws(":", col("triad"), col("adjustedCount")))
        formattedInvalidDF = invDF.withColumn("formatted", concat_ws("", lit("invalid line:"), col("count"))).withColumn("rank", lit(0))

        # Combine the two DFs together based on the dummy variables
        combinedDF = formattedInvalidDF.select("category", "formatted", "rank").union(formattedTopKDF.select("category", "formatted", "rank")).orderBy("category", "rank", "formatted")
        
        # Drop the dummy variable from the output
        outputDF = combinedDF.select("category", "formatted")

        # This section is for output
        outputDF.write.option("delimiter","\t").csv(outputPath)
        spark.stop()

if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("Wrong arguments")
        sys.exit(-1)
    Project2().run(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])

