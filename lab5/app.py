import streamlit as st
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import urllib.request

st.set_page_config(layout="wide")
st.title("Аналіз даних NOAA (Лабораторна №5)")

regions_dict = {
    1: 'Вінницька', 2: 'Волинська', 3: 'Дніпропетровська', 4: 'Донецька', 5: 'Житомирська',
    6: 'Закарпатська', 7: 'Запорізька', 8: 'Івано-Франківська', 9: 'Київська', 10: 'Кіровоградська',
    11: 'Луганська', 12: 'Львівська', 13: 'Миколаївська', 14: 'Одеська', 15: 'Полтавська',
    16: 'Рівненська', 17: 'Сумська', 18: 'Тернопільська', 19: 'Харківська', 20: 'Херсонська',
    21: 'Хмельницька', 22: 'Черкаська', 23: 'Чернівецька', 24: 'Чернігівська', 25: 'Республіка Крим'
}

@st.cache_data
def load_data():
    df_list = []
    for i in range(1, 26):
        url = f"https://www.star.nesdis.noaa.gov/smcd/emb/vci/VH/get_TS_admin.php?country=UKR&provinceID={i}&year1=2000&year2=2024&type=Mean"
        text = urllib.request.urlopen(url).read().decode('utf-8')
        text = text.replace("<br>", "").replace(",\n", "\n")
        lines = text.split('\n')[1:-1]
        data = [line.split(',') for line in lines if len(line.split(',')) == 7]
        
        temp_df = pd.DataFrame(data, columns=['year', 'week', 'SMN', 'SMT', 'VCI', 'TCI', 'VHI'])
        temp_df['Region_Name'] = regions_dict[i]
        df_list.append(temp_df)
        
    df = pd.concat(df_list, ignore_index=True)
    
    numeric_cols = ['year', 'week', 'SMN', 'SMT', 'VCI', 'TCI', 'VHI']
    for col in numeric_cols:
        df[col] = pd.to_numeric(df[col], errors='coerce')
        
    df = df[df['VHI'] > 0] 
    return df

df = load_data()

if 'reset' not in st.session_state:
    st.session_state.reset = False

def reset_all():
    st.session_state.idx = 'VHI'
    st.session_state.reg = 'Київська'
    st.session_state.years = (2010, 2020)
    st.session_state.weeks = (1, 52)
    st.session_state.asc = False
    st.session_state.desc = False

if st.button("Reset (Скинути фільтри)"):
    reset_all()

col1, col2 = st.columns([1, 3])

with col1:
    st.header("Фільтри")
    
    index_col = st.selectbox("Індекс:", ['VCI', 'TCI', 'VHI'], key='idx')
    region_col = st.selectbox("Область:", list(regions_dict.values()), key='reg')
    
    years_range = st.slider("Роки:", min_value=2000, max_value=2024, value=(2010, 2020), key='years')
    weeks_range = st.slider("Тижні:", min_value=1, max_value=52, value=(1, 52), key='weeks')
    
    st.write("Сортування:")
    check_asc = st.checkbox("За зростанням", key='asc')
    check_desc = st.checkbox("За спаданням", key='desc')

res_df = df[
    (df['Region_Name'] == region_col) &
    (df['year'].between(years_range[0], years_range[1])) &
    (df['week'].between(weeks_range[0], weeks_range[1]))
]

if check_asc and check_desc:
    st.warning("Увімкнено обидва чекбокси! Сортування за зростанням.")
    res_df = res_df.sort_values(by=index_col, ascending=True)
elif check_asc:
    res_df = res_df.sort_values(by=index_col, ascending=True)
elif check_desc:
    res_df = res_df.sort_values(by=index_col, ascending=False)

with col2:
    st.header("Дані та Графіки")
    tab1, tab2, tab3 = st.tabs(["Таблиця", "Графік динаміки", "Порівняння областей"])
    
    with tab1:
        st.dataframe(res_df[['year', 'week', 'Region_Name', 'VCI', 'TCI', 'VHI']])
        
    with tab2:
        if not res_df.empty:
            fig, ax = plt.subplots(figsize=(10, 5))
            
            res_df_sorted = res_df.sort_values(['year', 'week'])
            x_axis = res_df_sorted['year'].astype(str) + "-" + res_df_sorted['week'].astype(str)
            
            ax.plot(x_axis, res_df_sorted[index_col], color='b', label=index_col)
            ax.set_title(f"Динаміка {index_col} ({region_col})")
            ax.set_xlabel("Рік-Тиждень")
            ax.set_ylabel(index_col)
            
            ax.set_xticks(ax.get_xticks()[::len(ax.get_xticks())//10 + 1]) 
            plt.xticks(rotation=45)
            
            st.pyplot(fig)
        else:
            st.write("Немає даних для графіка")
            
    with tab3:
        df_all_regs = df[
            (df['year'].between(years_range[0], years_range[1])) &
            (df['week'].between(weeks_range[0], weeks_range[1]))
        ]
        mean_df = df_all_regs.groupby('Region_Name')[index_col].mean().reset_index()
        
        fig2, ax2 = plt.subplots(figsize=(10, 5))
        sns.barplot(data=mean_df, x='Region_Name', y=index_col, ax=ax2, color='lightblue')
        
        for p, item in zip(ax2.patches, mean_df['Region_Name']):
            if item == region_col:
                p.set_facecolor('red')
                
        ax2.set_title(f"Середній {index_col} по всіх областях")
        plt.xticks(rotation=90)
        st.pyplot(fig2)