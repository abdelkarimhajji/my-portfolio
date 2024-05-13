
import { BrowserRouter as Router, Route, Switch } from 'react-router-dom';
import { Link } from 'react-router-dom';
function AboutPage() {
  return (
    <>
    <h1>Home about</h1>
    <Link to="/"><button>home</button></Link>
    </>
  );
  }
  export default AboutPage;