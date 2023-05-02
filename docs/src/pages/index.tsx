import React from 'react';
import clsx from 'clsx';
import Link from '@docusaurus/Link';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';
import Layout from '@theme/Layout';

import styles from './index.module.css';

type FeatureItem = {
  title: string;
  Svg: React.ComponentType<React.ComponentProps<'svg'>>;
  description: JSX.Element;
};

const FeatureList: FeatureItem[] = [
  {
    title: 'Easy to Use',
    Svg: require('@site/static/img/working_from_anywhere-cuate.svg').default,
    description: (
      <>
        GBM was designed from the ground up to be easily installed and
        used to get your game up and running quickly.
      </>
    ),
  },
  {
    title: 'Focus on What Matters',
    Svg: require('@site/static/img/developer_activity-cuate.svg').default,
    description: (
      <>
        GBM lets you focus on your game, and we&apos;ll do the chores. Go
        ahead and download GBM plugin.
      </>
    ),
  },
  {
    title: 'Powered by the community',
    Svg: require('@site/static/img/all_the_data-cuate.svg').default,
    description: (
      <>
        See the open source project on github and let's give your feedback.
      </>
    ),
  },
];

export default function Home(): JSX.Element {
  const { siteConfig } = useDocusaurusContext();

  return (
    <Layout
      title={`Hello from ${siteConfig.title}`}
      description="Description will go into a meta tag in <head />">
      <header className={clsx('hero hero--primary', styles.heroBanner)}>
        <div className="container">
          <h1 className="hero__title">{siteConfig.title}</h1>
          <p className="hero__subtitle">{siteConfig.tagline}</p>
          <div className={styles.buttons}>
            <Link
              className="button button--secondary button--lg"
              to="/docs/intro">
              First Step Tutorial - 5min ⏱️
            </Link>
          </div>
        </div>
      </header>
      <main>

        <section className={styles.features}>
          <div className="container">
            <div className="row">

              {FeatureList.map(({ title, Svg, description }, idx) => (
                <div key={idx} className={clsx('col col--4')}>
                  <div className="text--center">
                    <Svg className={styles.featureSvg} role="img" />
                  </div>

                  <div className="text--center padding-horiz--md">
                    <h3>{title}</h3>

                    <p>{description}</p>
                  </div>
                </div>
              ))}

            </div>
          </div>
        </section>

      </main>
    </Layout>
  );
}
