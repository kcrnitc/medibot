import streamlit as st
import mysql.connector

# Initialize session state variables
if "patient_login_button" not in st.session_state:
    st.session_state["patient_login_button"] = False
if "hospital_admin_login_button" not in st.session_state:
    st.session_state["hospital_admin_login_button"] = False

# Set the title with custom styling
original_title = """
<h1 style="font-family: 'Arial Black', sans-serif; color: #ffffff; font-size: 40px; text-shadow: 2px 2px 4px #000000;">WELCOME TO MEDIBOT✨</h1>
"""
st.markdown(original_title, unsafe_allow_html=True)

# Set the background image
background_image = """
<style>
[data-testid="stAppViewContainer"] > .main {
    background-image: url("https://img.freepik.com/free-vector/medical-technology-science-background-vector-blue-with-blank-space_53876-117739.jpg?w=996&t=st=1710675337~exp=1710675937~hmac=8443e353dd8458d2625b6c63b2ba4f92a233d1015a7cff31d89227f86d4e1686");
    background-size: cover;  /* This sets the size to cover the entire area */
    background-position: center;  
    background-repeat: no-repeat;
}
</style>
"""

st.markdown(background_image, unsafe_allow_html=True)

# Function to establish database connection
def connect_to_database():
    try:
        conn = mysql.connector.connect(
            host="localhost",
            user="root",
            password="7square",
            database="medibot"
        )
        return conn
    except mysql.connector.Error as e:
        st.error(f"Error connecting to MySQL database: {e}")
        return None

# Function to validate login credentials
def validate_credentials(username, password):
    conn = connect_to_database()
    if conn:
        try:
            cursor = conn.cursor()
            query = "SELECT * FROM medibot WHERE username = %s AND password = %s"
            cursor.execute(query, (username, password))
            result = cursor.fetchone()
            if result:
                return True
            else:
                return False
        except mysql.connector.Error as e:
            st.error(f"Error executing MySQL query: {e}")
        finally:
            if conn.is_connected():
                cursor.close()
                conn.close()

# Function to display the login form
def display_login_form(role):
    st.write(f":violet[Please enter your credentials for {role} login:]")
    # Style the username and password input fields with custom color
    username_label = f":violet[Username]"
    password_label = f":violet[Password]"
    username = st.text_input(username_label, key=f"{role.lower()}_username", value="", type="default", help=None, placeholder="Enter username", autocomplete="default")
    password = st.text_input(password_label, key=f"{role.lower()}_password", value="", type="password", help=None, placeholder="Enter password", autocomplete="default")
    if st.button("Login"):
        if validate_credentials(username, password):
            st.success(f"Access granted! You are logged in as {role}")
            if role == "Patient":
                st.experimental_reroute("/patient_details_page")
            elif role == "Hospital Administration":
                st.experimental_reroute("/hospital_admin_details_page")
        else:
            st.error("Access denied! Invalid username or password.")

# Button 1: Login as Patient
if st.button("Login as Patient"):
    st.session_state["patient_login_button"] = True
    st.session_state["hospital_admin_login_button"] = False

# Button 2: Login as Hospital Administration
if st.button("Login as Hospital Administration"):
    st.session_state["hospital_admin_login_button"] = True
    st.session_state["patient_login_button"] = False

# Display login forms based on button clicks
if st.session_state["patient_login_button"]:
    display_login_form("Patient")
if st.session_state["hospital_admin_login_button"]:
    display_login_form("Hospital Administration")
