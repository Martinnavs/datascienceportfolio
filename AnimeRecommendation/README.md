# Anime Recommendation System using User-based Collaborative Filtering

## Preliminaries
The original datasets for this project come from [this Kaggle account](https://www.kaggle.com/datasets/CooperUnion/anime-recommendations-database). The datasets included here are mined from the original datasets.

## Data Mining
To be able to do recommendations using [collaborative filtering](https://builtin.com/data-science/collaborative-filtering-recommender-system) there is a need to establish a basis on which to compare users. For this project, 
it is called a preference profile. A preference profile is defined as the 8 favorite anime genres per user. However, since the datasets do not provide such profiles, data mining is conducted to generate these profiles from existing data.

The favorite anime genres are sourced from the animes with high user ratings, as it is assumed that a high rating implies that the user likes the anime and the genre transitively. Thus, the first step is to get the 5 highest-rated anime per user. It must be noted, however, that the number of anime that a certain user watches greatly varies within the dataset. This implies that some users may not reach 5 animes.

Then, the preference profile is created from these animes. It is done by getting the 8 most occuring genres in the user's 5 (at most) highest-rated anime. These genres are then turned into a binary string, which is based on the predefined arrangement of all genres in the anime dataset. As an example, if the arrangement is

    Action, Adventure, Comedy, Fantasy, Psychological

and the user's top 3 (in this case) genres are Action, Fantasy, and Psychological, the binary string will look like this:

    10010
    
This is done for all the users in the dataset who has a corresponding record in the mined top 5 animes dataset. 

The top 5 animes and preference profile of all users in the original datasets are saved into csv, which will be used in the recommendations.


## Recommendation

The anime recommendation system is based on the concept of collaborative filtering. It works by executing these steps:

1. Within a sample of 500 users, find 10 users with similar preference profiles (genres of interest) as the current user.
  - This is done through the hamming distance, which is defined as the number of bit positions within two binary strings in which the two bits are not the same. Translating this into the use case, it measures how similar the preferences of the user from other users are --- a greater value means that the user and another user has different preferences, while a lower value implies the opposite.
2. If the recommendation specifies highly rated shows only, then only the 5 highest rated animes per user will be included in the list. If not, then every anime that the 10 users have watched will be included in the list in random order.
3. Remove anime from the list that the current user has already seen.
4. Get the first 20 animes from the list as a recommendation for the user.
 
However, the method is not failproof:

- Since the number of anime that a user watches greatly varies, the number of anime included within the recommendations also depends on what the users with the most similar preferences have watched.
- Consequently, there may be the possibility that there will be fewer than 20 animes in the recommendation list.
- In the case where two animes have the same rating, the ordering will depend on how Pandas has sorted the list beforehand.
